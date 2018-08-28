#include "game.h"
#include "shader.h"
#include "texture.h"
#include <QMouseEvent>

static float xpos = 0, ypos = 0;

struct cubeStuff
{
	std::array<float,20> rotX, rotY, rotZ, posX, posY, posZ, scale;
	std::array<float,10> groupPos, groupRotX;
} cube;

GLuint VBO, VAO, EBO;
GLuint tex3ID;

float vertices[] = {
	// positions           // texture coords
	1.0f,  1.0f, 0.0f,        1.0f, 1.0f, // top right
	1.0f, -1.0f, 0.0f,        1.0f, 0.0f, // bottom right
	-1.0f, -1.0f, 0.0f,       0.0f, 0.0f, // bottom left
	-1.0f,  1.0f, 0.0f,       0.0f, 1.0f  // top left 
};

unsigned int indices[] = {
	0, 1, 3, // first triangle
	1, 2, 3  // second triangle
};

static ShaderProgram* sprogram;

glw::Texture* img1;
glw::Texture* img2;
glw::Texture* img3;

float rand(float low, float high)
{
	return (high - low) * (float)(std::rand())/(float)RAND_MAX + low;
}


Game::Game(QWidget* parent)
	: QOpenGLWidget(parent)
{
	QSurfaceFormat format;
	format.setVersion(3, 3);
	format.setProfile(QSurfaceFormat::CoreProfile);
	this->setFormat(format);
}

void Game::initializeGL()
{
	ctx = new QOpenGLFunctions_3_3_Core();
	ctx->initializeOpenGLFunctions();

	// init cube	
	{
		for (int i = 0; i < cube.rotX.size(); ++i)
		{
			cube.rotX[i] = 6.28f * (float)(std::rand()) / (float)RAND_MAX;
			cube.rotY[i] = 6.28f * (float)(std::rand()) / (float)RAND_MAX;
			cube.rotZ[i] = 6.28f * (float)(std::rand()) / (float)RAND_MAX;
			cube.posX[i] = 5.0f * rand(-1.0f, +1.0f);
			cube.posY[i] = 5.0f * rand(-1.0f, +1.0f);
			cube.posZ[i] = 3.0f * (float)(std::rand()) / (float)RAND_MAX;
			cube.scale[i] = 0.3f + 0.7f * (float)(std::rand()) / (float)RAND_MAX;
		}
		for (int i = 0; i < 20; ++i)
		{

		}
	}

	Profiler::get().add("Set Pixels");
	Profiler::get().add("Send Pixels");

	// create + compile shaders ; add to shader program
	sprogram = new ShaderProgram(ctx);
	sprogram->setVertexShader(std::make_shared<VertexShader>(ctx, "shaders/VS_math.glsl"));
	sprogram->setFragmentShader(std::make_shared<FragmentShader>(ctx, "shaders/FS_math.glsl"));
	// link shaders
	sprogram->compile();
	sprogram->setActive();

	ctx->glEnable(GL_DEPTH_TEST);
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

	img1 = new glw::Texture(ctx, "textures/container.jpg");
	img2 = new glw::Texture(ctx, "textures/awesomeface.png");
	img3 = new glw::Texture(ctx,100,100);

	sprogram->setTextureLocation("ourTexture1", *img1);
	sprogram->setTextureLocation("ourTexture2", *img2);
	sprogram->setTextureLocation("ourTexture3", *img3);

	timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(update()));
	timer->start(33);
}

void Game::resizeGL(int width, int height)
{
	//
}

void Game::paintGL()
{
	static Timer timer2;

	GLuint transformLoc = ctx->glGetUniformLocation(sprogram->m_program_id, "view");


	Profiler::get().resetTimer("Set Pixels");


	uint32_t* tmp2 = &img3->rawPixels()[0];

	for (size_t y = 0; y < img3->height(); ++y)
	{
		for (size_t x = 0; x < img3->width(); ++x)
		{// G B R A
			//auto tmp = glw::rgba(uint8_t(128 - mouse.y() * 127) + std::rand(), uint8_t(mouse.y() * 127 - 128), uint8_t(mouse.x() * 255), 0);
			tmp2[x + y * img3->width()] = glw::rgba(255*xpos, 128 + 128*ypos, 128 * ypos - 128, 0);


		}
	}

	img1->glwActiveTexture();
	img1->glwBindTexture();
	img2->glwActiveTexture();
	img2->glwBindTexture();
	img3->glwActiveTexture();
	img3->glwBindTexture();

	img3->sendToGPU();

	//sprogram.setActive();
	Profiler::get().recordTime("Send Pixels");

	// TODO: move these
	ctx->glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	ctx->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	ctx->glBindVertexArray(VAO);

	auto drawCube = [&]()
	{

		//rotateX(mouse.x() * 3);
		//rotateY(mouse.y() * 3);
		m.pushMatrix();
		ctx->glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(m.getModelView()));
		ctx->glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		m.popMatrix();
		m.pushMatrix();
		m.translate({ 0,0,2 });
		ctx->glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(m.getModelView()));
		ctx->glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		m.popMatrix();
		m.pushMatrix();
		m.translate({ 0,1,1 });
		m.rotateX(glm::radians(90.0f));

		ctx->glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(m.getModelView()));
		ctx->glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		m.popMatrix();
		m.pushMatrix();
		m.translate({ 0,-1,1 });
		m.rotateX(glm::radians(90.0f));

		ctx->glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(m.getModelView()));
		ctx->glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		m.popMatrix();
		m.pushMatrix();
		m.translate({ 1,0,1 });
		m.rotateY(glm::radians(90.0f));

		ctx->glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(m.getModelView()));
		ctx->glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		m.popMatrix();
		m.pushMatrix();
		m.translate({ -1,0,1 });
		m.rotateY(glm::radians(90.0f));
		ctx->glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(m.getModelView()));
		ctx->glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		m.popMatrix();

	};

	m.pushMatrix();
	m.scale(0.2);
	//m.rotateX(mouse.x() * 3);
	//m.rotateY(mouse.y() * 3);
	for (int i = 0; i < cube.scale.size(); ++i)
	{
		m.pushMatrix();
		m.translate({ 0.1*cube.posX[i], 0.1*cube.posY[i], 0.1*cube.posZ[i] });
		m.scale(cube.scale[i]);
		m.rotateX(cube.rotX[i] + std::sin(timer2.getDuration()));
		m.rotateY(cube.rotY[i] + std::cos(timer2.getDuration()));
		m.rotateZ(cube.rotZ[i]);
		m.translate({ cube.posX[i], cube.posY[i], cube.posZ[i] });



		for (int j = 0; j < 1; ++j)
		{
			//rotateX(groupRotX[j])
			//translate(groupPos[i]);
			//glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(getModelView()));

			//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			drawCube();
		}
		m.popMatrix();
	}
	m.popMatrix();
	ctx->glBindVertexArray(0);
}

void Game::mousePressEvent(QMouseEvent * event)
{
}

void Game::mouseMoveEvent(QMouseEvent* event)
{
	xpos = event->x() / float(this->width());
	ypos = event->y() / float(this->height());
}

void Game::mouseDoubleClickEvent(QMouseEvent * event)
{
}
