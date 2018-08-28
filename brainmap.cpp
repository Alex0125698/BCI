#include "brainmap.h"
#include "state.h"
#include <QTimer>
#include <QMouseEvent>

float vertices3[] = {
	// positions           // texture coords
	1.0f, 1.0f, 0.0f,        1.0f, 1.0f, // top right
	1.0f, -1.0f, 0.0f,        1.0f, 0.0f, // bottom right
	-1.0f, -1.0f, 0.0f,       0.0f, 0.0f, // bottom left
	-1.0f,  1.0f, 1.0f,       0.0f, 1.0f  // top left 
};

unsigned int indices3[] = {
	0, 1, 3, // first triangle
	1, 2, 3  // second triangle
};

BrainMap::BrainMap(QWidget* parent)
	: QOpenGLWidget(parent)
{
	QSurfaceFormat format;
	format.setVersion(3, 3);
	format.setProfile(QSurfaceFormat::CoreProfile);
	this->setFormat(format);
}

void BrainMap::initializeGL()
{
	ctx = new QOpenGLFunctions_3_3_Core();
	ctx->initializeOpenGLFunctions();

	ctx->glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	ctx->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// create + compile shaders ; add to shader program
	m_sprogram = new ShaderProgram(ctx);
	m_sprogram->setVertexShader(std::make_shared<VertexShader>(ctx, "shaders/VS_brainmap.glsl"));
	m_sprogram->setFragmentShader(std::make_shared<FragmentShader>(ctx, "shaders/FS_brainmap.glsl"));
	// link shaders
	m_sprogram->compile();
	m_sprogram->setActive();

	//ctx->glEnable(GL_DEPTH_TEST);
	ctx->glGenVertexArrays(1, &VAO);
	ctx->glGenBuffers(1, &VBO);

	ctx->glGenBuffers(1, &EBO);

	ctx->glBindVertexArray(VAO);
	{
		ctx->glBindBuffer(GL_ARRAY_BUFFER, VBO);
		ctx->glBufferData(GL_ARRAY_BUFFER, sizeof(vertices3), &vertices3[0], GL_STATIC_DRAW);

		ctx->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		ctx->glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices3), &indices3[0], GL_STATIC_DRAW);

		ctx->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
		ctx->glEnableVertexAttribArray(0);

		ctx->glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		ctx->glEnableVertexAttribArray(1);
	}
	ctx->glBindVertexArray(0);

	m_pixels = new glw::Texture(ctx, "textures/test.png"); // new glw::Texture(ctx, 500, 500);
	m_head = new glw::Texture(ctx, "textures/test.png");


	m_sprogram->setTextureLocation("ourTexture1", *m_pixels);
	m_sprogram->setTextureLocation("ourTexture2", *m_head);

	QTimer* timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(update()));
	timer->start(100);
}

void BrainMap::resizeGL(int width, int height)
{
}

void BrainMap::paintGL()
{
	ctx->glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	ctx->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	static int counter = 0;
	++counter;

	if (counter >= DTFT_Shared_Data.timePoints) counter = 0;

	m_sprogram->setActive();

	std::fill(m_pixels->rawPixels().begin(), m_pixels->rawPixels().end(), 0xFFFFFFFF);

	uint32_t* tmp2 = &m_pixels->rawPixels()[0];

	DTFT_Shared_Data;

	int xsize = m_pixels->width() / DTFT_Shared_Data.timePoints;
	int ysize = m_pixels->height() / DTFT_Shared_Data.freqPoints;

	uint32_t col = 0;

	float xpos2 = xpos * m_pixels->width();
	float ypos2 = ypos * m_pixels->height();

	int xstart = (int(xpos*m_pixels->width()) - 60 >= 0) ? int(xpos*m_pixels->width()) - 60 : 0;
	int xstop = (int(xpos*m_pixels->width()) + 60 < m_pixels->width()) ? int(xpos*m_pixels->width()) + 60 : m_pixels->width()-1;
	
	int ystart = (int(ypos*m_pixels->height()) - 60 >= 0) ? int(ypos*m_pixels->height()) - 60 : 0;
	int ystop = (int(ypos*m_pixels->height()) + 60 < m_pixels->height()) ? int(ypos*m_pixels->height()) + 60 : m_pixels->height()-1;

	for (size_t y = ystart; y < ystop; ++y)
	{

		for (size_t x = xstart; x <xstop; ++x)
		{
			float mag = sqrt((x - xpos2)*(x - xpos2) + (y - ypos2)*(y - ypos2)) / (1.41*60.0);
			tmp2[x + y * m_pixels->width()] = glw::rgba(mag*250, mag * 250,255,0);
		}
	}

	m_pixels->glwActiveTexture();
	m_pixels->glwBindTexture();
	m_head->glwActiveTexture();
	m_head->glwBindTexture();
	m_pixels->sendToGPU();

	ctx->glBindVertexArray(VAO);
	ctx->glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	ctx->glBindVertexArray(0);
}

void BrainMap::mousePressEvent(QMouseEvent* event)
{
}

void BrainMap::mouseMoveEvent(QMouseEvent* event)
{
	xpos = event->x() / float(this->width());
	ypos = 1 - event->y() / float(this->height());
}

void BrainMap::mouseDoubleClickEvent(QMouseEvent* event)
{
}
