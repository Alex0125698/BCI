#include "resources.h"
#include "balltest.h"
#include <QTimer>
#include <QMouseEvent>

float BallTest::vertices[] = {
	// positions           // texture coords
	 1.0f,  1.0f, 0.0f,       1.0f, 1.0f, // top right
	 1.0f, -1.0f, 0.0f,       1.0f, 0.0f, // bottom right
	-1.0f, -1.0f, 0.0f,       0.0f, 0.0f, // bottom left
	-1.0f,  1.0f, 1.0f,       0.0f, 1.0f  // top left 
};
unsigned int BallTest::indices[] = {
	0, 1, 3, // first triangle
	1, 2, 3  // second triangle
};

BallTest::BallTest(QWidget* parent)
	: QOpenGLWidget(parent)
{
	QSurfaceFormat format;
	format.setVersion(3, 3);
	format.setProfile(QSurfaceFormat::CoreProfile);
	this->setFormat(format);
}

void BallTest::initializeGL()
{
	ctx = new QOpenGLFunctions_3_3_Core();
	ctx->initializeOpenGLFunctions();
	ctx->glEnable(GL_BLEND);
	ctx->glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// create + compile shaders ; add to shader program
	m_sprogram = new ShaderProgram(ctx);
	m_sprogram->setVertexShader(std::make_shared<VertexShader>(ctx, "shaders/VS_balltest.glsl"));
	m_sprogram->setFragmentShader(std::make_shared<FragmentShader>(ctx, "shaders/FS_balltest.glsl"));
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
	m_ballTex = new glw::Texture(ctx, "images/basketball.png");
	m_ballTex->sendToGPU();
	m_sprogram->setTextureLocation("balltex", *m_ballTex);

	// setup timer to provide continuous drawing
	QTimer* timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(update()));
	timer->start(33);
}

void BallTest::resizeGL(int width, int height)
{

}

void BallTest::paintGL()
{
	// send pixels to GPU
	int posLoc = ctx->glGetUniformLocation(m_sprogram->m_program_id, "pos");
	int aspectRatioLoc = ctx->glGetUniformLocation(m_sprogram->m_program_id, "aspectRatio");
	m_sprogram->setActive();
	ctx->glUniform2f(posLoc,(GLfloat)xpos,(GLfloat)ypos);
	ctx->glUniform1f(aspectRatioLoc,(GLfloat)this->width()/(GLfloat)this->height());
	
	m_ballTex->basic.glwActiveTexture();
	m_ballTex->basic.glwBindTexture();
	
	// draw pixles inside of rect
	ctx->glBindVertexArray(VAO);
	ctx->glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	ctx->glBindVertexArray(0);

	ballMovement(xd, yd);
}

void BallTest::mouseMoveEvent(QMouseEvent* event)
{
	xd = -(2.0*event->x() / float(this->width()) - 1.0);
	yd = -(1.0 - 2.0*event->y() / float(this->height()));
}

void BallTest::ballMovement(double leftControl, double rightControl)
{
	static double vel1 = 0.0, vel2 = 0.0;
	static double pos1 = 0.0, pos2 = 0.0;

	vel1 = leftControl * (pos1 - 1.0) * 0.20;
	vel2 = rightControl * (pos2 - 1.0) * 0.20;

	pos1 += vel1;
	pos2 += vel2;

	pos1 *= 0.985;
	pos2 *= 0.985;

	if (pos1 < 0.0) pos1 = 0.0;
	if (pos2 < 0.0) pos2 = 0.0;

	if (pos1 > 0.95) pos1 = 0.95;
	if (pos2 > 0.95) pos2 = 0.95;

	ypos = 2.0*(pos1+pos2 - pos1*pos2) - 1.0;
	xpos = (pos1 - pos2);
}
