#include "brainmap.h"
#include "state.h"
#include <QTimer>
#include <QMouseEvent>

float BrainMap::vertices[] = {
	// positions           // texture coords
	1.0f, 1.0f, 0.0f,        1.0f, 1.0f, // top right
	1.0f, -1.0f, 0.0f,        1.0f, 0.0f, // bottom right
	-1.0f, -1.0f, 0.0f,       0.0f, 0.0f, // bottom left
	-1.0f,  1.0f, 1.0f,       0.0f, 1.0f  // top left 
};
unsigned int BrainMap::indices[] = {
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

	// create + compile shaders ; add to shader program
	m_sprogram = new ShaderProgram(ctx);
	m_sprogram->setVertexShader(std::make_shared<VertexShader>(ctx, "shaders/VS_brainmap.glsl"));
	m_sprogram->setFragmentShader(std::make_shared<FragmentShader>(ctx, "shaders/FS_brainmap.glsl"));
	// link shaders
	m_sprogram->compile();
	m_sprogram->setActive();

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

	m_pixels = new glw::Texture(ctx, "textures/test.png");
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
	m_sprogram->setActive();
	m_pixels->fillColor(0xFFFFFFFF);

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
			//float mag = sqrt((x - xpos2)*(x - xpos2) + (y - ypos2)*(y - ypos2)) / (1.41*60.0);
			//tmp2[x + y * m_pixels->width()] = glw::color(mag*250, mag * 250,255,0);
		}
	}

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
