#pragma once

#include "resources.h"
#include "openglresources.h"
#include "shader.h"
#include "texture.h"
#include <stack>

class BrainMap : public QOpenGLWidget
{
	Q_OBJECT

public:
	BrainMap(QWidget* parent);
	void initializeGL() override;
	void resizeGL(int width, int height) override;
	void paintGL() override;
	void mousePressEvent(QMouseEvent *event) override;
	void mouseMoveEvent(QMouseEvent *event) override;
	void mouseDoubleClickEvent(QMouseEvent *event) override;

private:
	QOpenGLFunctions_3_3_Core* ctx{ nullptr };
	ShaderProgram* m_sprogram{ nullptr };
	glw::Texture* m_pixels{ nullptr };
	glw::Texture* m_head{ nullptr };
	GLuint VBO, VAO, EBO;
	float xpos = 0, ypos = 0;

private:
	static float vertices[];
	static unsigned int indices[];
};

