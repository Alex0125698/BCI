#include "resources.h"
#include "pixplotter.h"
#include "controllerstate.h"
#include "mainwindowstate.h"
#include <QPainter>

float PixPlotter::vertices[] = {
	// positions           // texture coords
	 1.0f,  1.0f, 0.0f,       1.0f, 1.0f, // top right
	 1.0f, -1.0f, 0.0f,       1.0f, 0.0f, // bottom right
	-1.0f, -1.0f, 0.0f,       0.0f, 0.0f, // bottom left
	-1.0f,  1.0f, 1.0f,       0.0f, 1.0f  // top left 
};
unsigned int PixPlotter::indices[] = {
	0, 1, 3, // first triangle
	1, 2, 3  // second triangle
};

PixPlotter::PixPlotter(QWidget* parent)
	: QOpenGLWidget(parent)
{
	// setup timer to provide continuous drawing
	timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(updateTexture()));
}

void PixPlotter::initializeGL()
{
	ctx = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_3_Core>();
	ctx->initializeOpenGLFunctions();

	// create + compile shaders ; add to shader program
	m_sprogram = new ShaderProgram(ctx);
	m_sprogram->setVertexShader(std::make_shared<VertexShader>(ctx, "shaders/VS_pixplotter.glsl"));
	m_sprogram->setFragmentShader(std::make_shared<FragmentShader>(ctx, "shaders/FS_pixplotter.glsl"));
	// link shaders
	m_sprogram->compile();
	m_sprogram->setActive();

	// a buch of stuff to draw a simple rect
	ctx->glGenVertexArrays(1, &VAO);
	ctx->glGenBuffers(1, &VBO);
	ctx->glGenBuffers(1, &EBO);

	ctx->glBindVertexArray(VAO);
	{
		ctx->glBindBuffer(GL_ARRAY_BUFFER, VBO);
		ctx->glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices[0], GL_STATIC_DRAW);
		ctx->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		ctx->glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), &indices[0], GL_STATIC_DRAW);
		ctx->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
		ctx->glEnableVertexAttribArray(0);
		ctx->glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		ctx->glEnableVertexAttribArray(1);
	}
	ctx->glBindVertexArray(0);

	// setup texture
	m_pixels = new PixTextureFlip(ctx, 100, 128);
	m_pixels->fillColor(glw::color(5, 5, 15));
	m_pixels->sendToGPU();
	m_sprogram->setTextureLocation("ourTexture1", *m_pixels);

	connect(timer, SIGNAL(timeout()), this, SLOT(update()));
	timer->start(100);
}

void PixPlotter::resizeGL(int width, int height)
{
	width; height;
}

void PixPlotter::checkState()
{
	{
		std::lock_guard<std::mutex> lock(ControllerState::state.mtx_data);

		if (ControllerState::state.resetSTFT)
			state.resetSTFT = true;
		ControllerState::state.resetSTFT = false;
		state.imgOutputFD = std::move(ControllerState::state.imgOutputFD);
		ControllerState::state.imgOutputFD.clear();
		state.freq = ControllerState::state.freq;
		state.stft_plot_time = ControllerState::state.stft_plot_time;
		state.timeLerp = ControllerState::state.timeLerp;
	}

	{
		std::lock_guard<std::mutex> lock(MainWindowState::state.mtx_data);
		dtft = MainWindowState::state.dtft;
	}
}

// todo: reset signal
void PixPlotter::updateTexture()
{
	//makeCurrent();
	// store our old properties for comparison
	DTFTproperties old_dtft = dtft;

	checkState();

	// no data means we are done
	if (state.imgOutputFD.size() == 0) return;

	// check if freq/time dim sizes have changed
	size_t reqTime = dtft.timeSpan + 1; // in seconds
	size_t reqTimePoints = (size_t(reqTime / state.timeLerp) & 0xFFFFFFFEu) + 2; // in num points
	size_t reqFreqPoints = state.imgOutputFD[0][0].size(); // index
	
	// we need to ajust the gain until the pixels are visible
	double gain = 5.0*std::pow(10.0, dtft.brightness*8.0 - 3.0);

	// resize pixel buffer + delete old pixels
	if (reqTimePoints != m_numTimepoints || (reqFreqPoints != m_numFreqpoints) || state.resetSTFT)
	{
		state.resetSTFT = false;
		m_pixels->resize(reqTimePoints, reqFreqPoints);
		m_pixels->fillColor(glw::color(5, 5, 15));
		m_numTimepoints = reqTimePoints;
		m_numFreqpoints = reqFreqPoints;
	}

	// copy the pixels
	// dim1 = buff ; dim2 = left/right ; dim3 = freq
	for (size_t t = 0; t < state.imgOutputFD.size(); ++t) // width
	{
		for (size_t freq = 0; freq < state.imgOutputFD[0][0].size(); ++freq) // height
		{
			// todo: this could be done in GPU if we use floating-point image
			double g = state.imgOutputFD[t][0][freq] * gain - dtft.hardLimit*255.0; // left
			double b = state.imgOutputFD[t][1][freq] * gain - dtft.hardLimit*255.0; // right
			g = std::clamp(g, 0.0, 255.0);
			b = std::clamp(b, 0.0, 255.0);
			auto r = std::clamp((b+g)/2.0, 0.0, 255.0);

			m_pixels->pixel(freq) = glw::color((uint8_t)r, (uint8_t)g, (uint8_t)b);// (r, g, b);
		}
		m_pixels->nextColumn();
	}
	//doneCurrent();
}

void PixPlotter::paintGL()
{
	{
		QPainter painter(this);
		painter.beginNativePainting();

		// send stuff to GPU
		m_sprogram->setActive();
		ctx->glUniform1f(ctx->glGetUniformLocation(m_sprogram->m_program_id, "range"), dtft.maxFreq);
		m_pixels->sendToGPU();

		// draw pixles inside of rect
		ctx->glBindVertexArray(VAO);
		ctx->glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		ctx->glBindVertexArray(0);

		painter.endNativePainting();
	}
	drawLabels();
}

void PixPlotter::drawLabels()
{
	QPainter painter(this);
	painter.setPen(QColor(100, 130, 210));
	QFont font("Times",9);
	font.setBold(true);
	painter.setFont(font);

	// === Frequency Labels ===
	{
		int xpos = 0.005*this->width() + 2;
		int blockSize = int(this->height() / m_numFreqpoints + 2);

		int num_freqs = 2 + ((8 * this->height()) / 550) & 0xFFFFFFFE;

		for (int i = 0; i < num_freqs; ++i)
		{
			int ypos = 8.0 + int(blockSize / 2.0 + (i / double(num_freqs - 1.0)) * (this->height() - blockSize - 8.0));
			int freq = int((dtft.maxFreq * state.freq * (num_freqs - i - 1.0)) / double(num_freqs - 1.0));

			painter.drawText(xpos, ypos, QString::number(freq) + " Hz");
		}
	}

	// === Time Labels ===
	{
		int ypos = int(this->height()*0.995 - 2);
		int blockSize = int(this->width() / m_numTimepoints);

		for (int i = 0; i < 10; ++i)
		{
			int xpos = int(this->width() * i / 10.0);
			if (xpos < 20) continue;
			painter.drawText(xpos,ypos, QString::number((i+1)*dtft.timeSpan/10.0 + state.stft_plot_time - dtft.timeSpan) + " S");
		}
	}
}
