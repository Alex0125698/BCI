#include "resources.h"
#include "brainmap.h"
#include <QTimer>
#include <QMouseEvent>
#include <QPainter>
#include <QFont>

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
	: QOpenGLWidget(parent),
	painter(this)
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

	ctx->glEnable(GL_BLEND);
	ctx->glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// create + compile shaders ; add to shader program
	m_sprogram = new ShaderProgram(ctx);
	m_sprogram->setVertexShader(std::make_shared<VertexShader>(ctx, "shaders/VS_brainmap.glsl"));
	m_sprogram->setFragmentShader(std::make_shared<FragmentShader>(ctx, "shaders/FS_brainmap.glsl"));
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

	m_brainTex = new glw::Texture(ctx, "textures/head_10_20.png");
	m_activityTex = new glw::Texture(ctx, "textures/activity.png");

	for (size_t i = 0; i < m_chTex.size(); ++i)
	{
		m_chTex[i] = new glw::Texture(ctx, "textures/ch" + std::to_string(i + 1) + ".png");
		m_sprogram->setTextureLocation(std::string("ch"+std::to_string(i+1)).c_str(), *m_chTex[i]);
	}
	
	m_sprogram->setTextureLocation("braintex", *m_brainTex);
	m_sprogram->setTextureLocation("activitytex", *m_activityTex);

	QTimer* timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(update()));
	timer->start(33);
}

void BrainMap::resizeGL(int width, int height)
{
}

void BrainMap::paintGL()
{
	//painter.begin(this);
	painter.beginNativePainting();

	int posLoc = ctx->glGetUniformLocation(m_sprogram->m_program_id, "pos");
	int sizeLoc = ctx->glGetUniformLocation(m_sprogram->m_program_id, "size");
	int useTexLoc = ctx->glGetUniformLocation(m_sprogram->m_program_id, "_useTex");
	int magLoc = ctx->glGetUniformLocation(m_sprogram->m_program_id, "mag");
	int chSelectLoc = ctx->glGetUniformLocation(m_sprogram->m_program_id, "chSelect");
	int aspectRatioLoc = ctx->glGetUniformLocation(m_sprogram->m_program_id, "aspectRatio");

	m_sprogram->setActive();
	ctx->glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

	m_brainTex->basic.glwActiveTexture();
	m_brainTex->basic.glwBindTexture();
	m_activityTex->basic.glwActiveTexture();
	m_activityTex->basic.glwBindTexture();

	for (size_t i = 0; i < m_chTex.size(); ++i)
	{
		m_chTex[i]->basic.glwActiveTexture();
		m_chTex[i]->basic.glwBindTexture();
	}

	ctx->glUniform2f(posLoc, (GLfloat)0.0, (GLfloat)0.0);
	ctx->glUniform1f(sizeLoc, 1.0);
	ctx->glUniform1f(useTexLoc, 1.0);
	ctx->glUniform1f(magLoc, 1.0);
	ctx->glUniform1f(aspectRatioLoc, GLfloat(width())/GLfloat(height()));

	ctx->glBindVertexArray(VAO);
	ctx->glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	ctx->glBindVertexArray(0);

	ctx->glBindVertexArray(VAO);
	for (int i = 0; i < elecManager.get().size(); ++i)
	{
		// draw red shading
		double x = elecManager.get()[i].pos.x;
		double y = elecManager.get()[i].pos.y;
		ctx->glUniform2f(posLoc, (GLfloat)(x), (GLfloat)(y));
		ctx->glUniform1f(sizeLoc, 0.4);
		ctx->glUniform1f(useTexLoc, 0.0);
		auto tmpMag = 1.0;// rand() / float(RAND_MAX);
		ctx->glUniform1f(magLoc, tmpMag);
		ctx->glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}

	for (int i = 0; i < elecManager.get().size(); ++i)
	{
		// draw channel name
		double x = elecManager.get()[i].pos.x;
		double y = elecManager.get()[i].pos.y;
		ctx->glUniform2f(posLoc, (GLfloat)(x), (GLfloat)(y));
		ctx->glUniform1f(sizeLoc, 0.08);
		ctx->glUniform1f(chSelectLoc, (GLfloat)(i % m_chTex.size()));
		ctx->glUniform1f(useTexLoc, 2.0);
		ctx->glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}

	ctx->glBindVertexArray(0);

	painter.endNativePainting();
	//painter.end();

	//this->makeCurrent();
	//this->doneCurrent();

	//drawLabels();
}

void BrainMap::drawLabels()
{
	painter.begin(this);
	painter.setPen(QColor(50,150,70));

	QFont font("Times", 8, QFont::Bold);
	font.setPixelSize(8);
	painter.setFont(font);

	for (size_t i = 0; i < elecManager.get().size(); ++i)
	{
		int x = int(width()*0.5*(1.0 + elecManager.get()[i].pos.x));
		int y = int(height()*0.5*(1.0 + -elecManager.get()[i].pos.y));

		painter.drawText(x, y, "Ch " + QString::number(i));
	}
	painter.end();
}

void BrainMap::mousePressEvent(QMouseEvent* event)
{
	float x = 2.0f * event->x() / (float)this->width() - 1.0f;
	float y = 1.0f - 2.0f * event->y() / (float)this->height();

	elecManager.testSelect({ x,y });
}

void BrainMap::mouseReleaseEvent(QMouseEvent* event)
{
	elecManager.deselect();
}

void BrainMap::mouseMoveEvent(QMouseEvent* event)
{
	float x = 2.0f * event->x() / (float)this->width() - 1.0f;
	float y = 1.0f - 2.0f * event->y() / (float)this->height();
	
	elecManager.moveElectrode({ x,y });
}