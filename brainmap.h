#pragma once

#include "resources.h"
#include "error.h"
#include "openglresources.h"
#include "shader.h"
#include "texture.h"
#include <stack>
#include <QPainter>

struct electrode
{
	glm::vec2 pos;
};

class electrodeManager
{
public:
	electrodeManager()
	{
		setElectrodeCount(16);
	}
	void setElectrodeCount(size_t n)
	{
		electrodes.resize(n);
		// generate positions across moor cortex
		for (size_t i = 0; i < electrodes.size(); ++i)
		{
			electrodes[i].pos.x = 0.9*(2.0*(i / ((float)electrodes.size() - 1.0f)) - 1.0);
			electrodes[i].pos.y = 0.16f * (2.0*double(i & 0x1u) - 1.0);
		}
	}
	void moveElectrode(glm::vec2 cursorPos)
	{
		assert(electrodes.size() > 0);

		if (selectOn)
			electrodes[selected].pos = cursorPos - off;
	}
	void testSelect(glm::vec2 cursorPos)
	{
		assert(electrodes.size() > 0);
		// check if any electrodes are within range
		// if so we select the one closest to the cursor
		const float elecRadius = 0.08;
		selectOn = false;

		size_t i = selected;	
		do
		{
			i = (i+1) % electrodes.size();
			float dist = glm::distance(electrodes[i].pos, cursorPos);
			if (dist <= elecRadius)
			{
				// we select the first potential electrode
				// but begin the search after the previous one
				// so tht it will cycle through
				selected = i;
				selectOn = true;
				off = cursorPos - electrodes[i].pos;
				break;
			}
			
		} while (i != selected);
		

	}
	const auto& get()
	{
		return electrodes;
	}
	void deselect()
	{
		selectOn = false;
	}

private:
	std::vector<electrode> electrodes;
	bool selectOn{ false };
	int selected{ 0 };
	glm::vec2 off;
};

class BrainMap : public QOpenGLWidget
{
	Q_OBJECT

public:
	BrainMap(QWidget* parent);
	void initializeGL() override;
	void resizeGL(int width, int height) override;
	void paintGL() override;
	void drawLabels();
	void mousePressEvent(QMouseEvent * event) override;
	void mouseReleaseEvent(QMouseEvent * event) override;
	void mouseMoveEvent(QMouseEvent *event) override;

private:
	QOpenGLFunctions_3_3_Core* ctx{ nullptr };
	ShaderProgram* m_sprogram{ nullptr };
	glw::Texture* m_activityTex{ nullptr };
	glw::Texture* m_brainTex{ nullptr };
	std::array<glw::Texture*, 3> m_chTex;
	GLuint VBO, VAO, EBO;
	electrodeManager elecManager;
	QPainter painter;

private:
	static float vertices[];
	static unsigned int indices[];
};

