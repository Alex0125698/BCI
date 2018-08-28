#pragma once

#include "resources.h"
#include "openglresources.h"
#include <stack>
#include <QTimer>

class MatrixSystem
{
public:
	void rotateX(float angle)
	{
		rotate(angle, { 1,0,0 });
	}
	void rotateY(float angle)
	{
		rotate(angle, { 0,1,0 });
	}
	void rotateZ(float angle)
	{
		rotate(angle, { 0,0,1 });
	}
	void rotate(float angle, glm::vec3&& axis)
	{
		rotate(angle, axis);
	}
	void rotate(float angle, glm::vec3& axis)
	{
		tos = glm::rotate(tos, angle, axis);
	}
	void translate(glm::vec3&& amount)
	{
		translate(amount);
	}
	void translate(glm::vec3& amount)
	{
		tos = glm::translate(tos, amount);
	}
	void scale(float amount)
	{
		scale({ amount,amount,amount });
	}
	void scale(glm::vec3&& amount)
	{
		scale(amount);
	}
	void scale(glm::vec3& amount)
	{
		tos = glm::scale(tos, amount);
	}
	void pushMatrix()
	{
		if (mats.size() > 2048) throw DETAILEDEXCEPTION("matrix stack has grown too large");

		mats.emplace(tos);
	}
	void popMatrix()
	{
		if (mats.empty()) throw DETAILEDEXCEPTION("cannot popMatrix, stack is empty");

		tos = mats.top();
		mats.pop();
	}
	constexpr const auto& getModelView()
	{
		return tos;
	}

private:
	glm::mat4 tos = glm::mat4(1.0);
	std::stack<glm::mat4> mats;
};

class Game : public QOpenGLWidget
{
	Q_OBJECT

public:
	Game(QWidget* parent);
	void initializeGL() override;
	void resizeGL(int width, int height) override;
	void paintGL() override;
	void mousePressEvent(QMouseEvent *event) override;
	void mouseMoveEvent(QMouseEvent *event) override;
	void mouseDoubleClickEvent(QMouseEvent *event) override;

public slots:
	void slotUpdate()
	{
		paintGL();
	}

private:
	MatrixSystem m;
	QOpenGLFunctions_3_3_Core* ctx;
	QTimer* timer;
};

