#include "pixplotter.h"
#include "state.h"
#include <QTimer>

float vertices2[] = {
	// positions           // texture coords
	1.0f, 1.0f, 0.0f,        1.0f, 1.0f, // top right
	1.0f, -1.0f, 0.0f,        1.0f, 0.0f, // bottom right
	-1.0f, -1.0f, 0.0f,       0.0f, 0.0f, // bottom left
	-1.0f,  1.0f, 1.0f,       0.0f, 1.0f  // top left 
};

unsigned int indices2[] = {
	0, 1, 3, // first triangle
	1, 2, 3  // second triangle
};

glw::Texture* img5;
glw::Texture* img4;

PixPlotter::PixPlotter(QWidget* parent)
	: QOpenGLWidget(parent)
{
	QSurfaceFormat format;
	format.setVersion(3, 3);
	format.setProfile(QSurfaceFormat::CoreProfile);
	this->setFormat(format);
}

void PixPlotter::initializeGL()
{
	ctx = new QOpenGLFunctions_3_3_Core();
	ctx->initializeOpenGLFunctions();

	ctx->glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	ctx->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// create + compile shaders ; add to shader program
	m_sprogram = new ShaderProgram(ctx);
	m_sprogram->setVertexShader(std::make_shared<VertexShader>(ctx, "shaders/VS_pixplotter.glsl"));
	m_sprogram->setFragmentShader(std::make_shared<FragmentShader>(ctx, "shaders/FS_pixplotter.glsl"));
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
		ctx->glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), &vertices2[0], GL_STATIC_DRAW);

		ctx->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		ctx->glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices2), &indices2[0], GL_STATIC_DRAW);

		ctx->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
		ctx->glEnableVertexAttribArray(0);

		ctx->glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		ctx->glEnableVertexAttribArray(1);
	}
	ctx->glBindVertexArray(0);

	m_pixels = new glw::Texture(ctx, "textures/awesomeface.png"); // new glw::Texture(ctx, 500, 500);
	img4 = new glw::Texture(ctx, "textures/awesomeface.png");
	m_sprogram->setTextureLocation("ourTexture1", *m_pixels);

	QTimer* timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(update()));
	timer->start(100);
}

void PixPlotter::resizeGL(int width, int height)
{
}

void PixPlotter::paintGL()
{
	ctx->glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	ctx->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	static int counter = 0;
	++counter;

	if (counter >= DTFT_Shared_Data.timePoints) counter = 0;

	m_sprogram->setActive();
	uint32_t* tmp2 = &m_pixels->rawPixels()[0];

	DTFT_Shared_Data;

	int xsize = m_pixels->width() / DTFT_Shared_Data.timePoints;
	int ysize = m_pixels->height() / DTFT_Shared_Data.freqPoints; 

	uint32_t col = 0;
	for (size_t y = 0; y < m_pixels->height(); ++y)
	{
		
		if (y % ysize == 0)
		{
			int r = std::rand() % 255;
			col = glw::rgba(r*0.9, r, r, 0);
		}
		for (size_t x = counter*xsize; x < (counter+1)*xsize; ++x)
		{
			tmp2[x + y * m_pixels->width()] = col;
		}
	}

	m_pixels->glwActiveTexture();
	m_pixels->glwBindTexture();
	img4->glwActiveTexture();
	img4->glwBindTexture();
	m_pixels->sendToGPU();	

	ctx->glBindVertexArray(VAO);
	ctx->glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	ctx->glBindVertexArray(0);
}

void PixPlotter::mousePressEvent(QMouseEvent* event)
{
}

void PixPlotter::mouseMoveEvent(QMouseEvent* event)
{
}

void PixPlotter::mouseDoubleClickEvent(QMouseEvent* event)
{
}
