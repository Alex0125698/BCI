#pragma once

#include "resources.h"
#include "openglresources.h"

class BallTest : public QOpenGLWidget
{
	Q_OBJECT

public:
	BallTest(QWidget* parent);
	virtual ~BallTest()
	{
		makeCurrent();
		if (m_sprogram)
		{
			delete m_sprogram;
			m_sprogram = nullptr;
		}
		if (m_ballTex)
		{
			delete m_ballTex;
			m_ballTex = nullptr;
		}
		doneCurrent();
	}
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
	GLuint VBO = 0, VAO = 0, EBO = 0;
	double xpos = 0, ypos = 0;
	double xd = 0, yd = 0;

private:
	static float vertices[];
	static unsigned int indices[];
	QTimer* timer{ nullptr };
};

