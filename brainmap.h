#pragma once

#include "resources.h"
#include "openglresources.h"
#include "brainmapstate.h"
#include <stack>

class QWidget;
class QVBoxLayout;
class QHBoxLayout;
class QComboBox;
class QToolButton;
class QSlider;

class TextureGroup
{
public:
	TextureGroup(QOpenGLFunctions_3_3_Core* ctx)
		:ctx(ctx)
	{
	}
	~TextureGroup()
	{
		for (auto& ptr : textures)
		{
			if (ptr)
			{
				delete ptr;
				ptr = nullptr;
			}
		}
	}

	void addTexture(std::string&& path)
	{
		glw::Texture* tmp = new glw::Texture(ctx, std::move(path),2);
		textures.push_back(tmp);
	}
	void setActive(size_t index)
	{
		textures[index]->basic.glwActiveTexture();
		textures[index]->basic.glwBindTexture();
	}
	auto* getTexture(size_t index)
	{
		return textures[index];
	}

private:
	QOpenGLFunctions_3_3_Core* ctx{ nullptr };
	std::vector<glw::Texture*> textures;
};

struct electrode
{
	glm::vec2 pos;
};

class electrodeManager
{
public:
	electrodeManager()
	{
		//setElectrodeCount(16);
	}
	auto getElectrodeCount()
	{
		return electrodes.size();
	}
	void setElectrodeCount(size_t n)
	{
		chCAR_TR.resize(n,0.0);
		elecStateRight.resize(n, 1.0);
		elecStateLeft.resize(n, 1.0);
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
		sassert(electrodes.size() > 0);

		if (selectOn)
			electrodes[selected].pos = cursorPos - off;
	}
	void testSelect(glm::vec2 cursorPos)
	{
		sassert(electrodes.size() > 0);
		// check if any electrodes are within range
		// if so we select the one closest to the cursor
		const float elecRadius = 0.08f;
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
				selected = (int)i;
				selectOn = true;
				off = cursorPos - electrodes[i].pos;
				break;
			}
			
		} while (i != selected);
		

	}
	void switchColor(bool left)
	{
		if (!selectOn) return;
		if (left)
		{
			if (elecStateLeft[selected] > 0.0)
				elecStateLeft[selected] = -1.0;
			else if (elecStateLeft[selected] < 0.0)
				elecStateLeft[selected] = 0.0;
			else
				elecStateLeft[selected] = 1.0;
		}
		else
		{
			if (elecStateRight[selected] > 0.0)
				elecStateRight[selected] = -1.0;
			else if (elecStateRight[selected] < 0.0)
				elecStateRight[selected] = 0.0;
			else
				elecStateRight[selected] = 1.0;
		}
	}
	auto getColor(int index, bool left)
	{
		if (left)
			return elecStateLeft[index];
		else
			return elecStateRight[index];
	}
	const auto& get()
	{
		return electrodes;
	}
	void deselect()
	{
		selectOn = false;
	}
	bool isSelected(int index)
	{
		sassert((unsigned)index < electrodes.size());
		return selectOn && (selected == index);
	}

private:
	std::vector<electrode> electrodes;
	bool selectOn{ false };
	int selected{ 0 };
	glm::vec2 off;
	// -1 = subtract ; 0 = ignore ; +1 = add

public:
	std::vector<double> chCAR_TR;
	std::vector<double> elecStateRight;
	std::vector<double> elecStateLeft;
};

class BrainMap : public QOpenGLWidget
{
	Q_OBJECT

public:
	BrainMap(QWidget* parent);
	virtual ~BrainMap()
	{
		makeCurrent();
		if (m_sprogram)
		{
			delete m_sprogram;
			m_sprogram = nullptr;
		}
		if (m_activityTex)
		{
			delete m_activityTex;
			m_activityTex = nullptr;
		}
		if (m_brainTex)
		{
			delete m_brainTex;
			m_brainTex = nullptr;
		}
		doneCurrent();
	}
	void initializeGL() override;
	void resizeGL(int width, int height) override;
	void exportState();
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
	TextureGroup* m_textures{ nullptr };
	GLuint VBO = 0, VAO = 0, EBO = 0;
	electrodeManager elecManager;

private:
	static float vertices[];
	static unsigned int indices[];

private:
	BrainMapState* state{ &BrainMapState::state };

private:
	struct GUIelements
	{
		QWidget* container{ nullptr };
		QVBoxLayout* contLayout{ nullptr };
		QHBoxLayout* r1Layout{ nullptr };
		QHBoxLayout* r2Layout{ nullptr };
		QHBoxLayout* r3Layout{ nullptr };
		QHBoxLayout* r4Layout{ nullptr };
		QHBoxLayout* r5Layout{ nullptr };
		QComboBox* box_filterDisp{ nullptr };
		QComboBox* box_filter1{ nullptr };
		QComboBox* box_filter2{ nullptr };
		QToolButton* btn_color{ nullptr };
		QSlider* slider_head{ nullptr };
		QSlider* slider_elec{ nullptr };
	} gui;
};

