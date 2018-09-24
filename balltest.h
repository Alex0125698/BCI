#pragma once

#include "resources.h"
#include "error.h"
#include "openglresources.h"
#include "shader.h"
#include "texture.h"


class BallTest : public QOpenGLWidget
{
	Q_OBJECT

public:
	BallTest(QWidget* parent);
	void initializeGL() override;
	void resizeGL(int width, int height) override;
	void paintGL() override;
	void mouseMoveEvent(QMouseEvent *event) override;

public:
	void ballMovement(double leftControl, double rightControl);

private:
	QOpenGLFunctions_3_3_Core* ctx{ nullptr };
	ShaderProgram* m_sprogram{ nullptr };
	glw::Texture* m_ballTex{ nullptr };
	GLuint VBO, VAO, EBO;
	double xpos = 0, ypos = 0;
	double xd = 0, yd = 0;

private:
	static float vertices[];
	static unsigned int indices[];
};

