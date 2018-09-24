#pragma once

#include "resources.h"
#include "error.h"
#include "openglresources.h"
#include "shader.h"
#include "texture.h"
#include "mainwindowstate.h"
#include <stack>

class PixTexture : public glw::Texture
{
public:
	PixTexture(QOpenGLFunctions_3_3_Core* ctx, size_t width, size_t height)
		: glw::Texture(ctx)
	{
		resize(width, height);
	}

	void resize(size_t width, size_t height)
	{
		m_width = width;
		m_height = height;
		m_maxHeight = height * 2;
		m_pixels.resize(m_width*m_maxHeight);
		m_resize = true;
		m_currOffset = 0;
		m_currRow = 0;
	}

	void sendToGPU()
	{
		assert(m_currOffset <= m_height);
		basic.glwActiveTexture();
		basic.glwBindTexture();
		if (m_resize)
		{
			basic.glwTexImage2D((GLsizei)m_width, (GLsizei)m_height, (GLvoid*)&m_pixels[0]);
			m_resize = false;
		}
		basic.glwTexSubImage2D((GLsizei)m_width, (GLsizei)m_height, (GLvoid*)&m_pixels[m_currOffset*m_width]);
	}

	void nextRow()
	{
		++m_currRow;

		if (m_currRow >= m_height)
		{
			++m_currOffset;
		}

		if (m_currRow >= m_maxHeight)
		{
			shiftData();
		}

		if (m_currRow >= m_maxHeight)
			assert(m_currRow < m_maxHeight);
		if (m_currOffset >= m_height+1)
			assert(m_currOffset < m_height+1);
	}

	void shiftData()
	{
		assert(m_height % 2 == 0);
		auto startLastHalf  = m_pixels.begin()+(m_width*m_height);
		auto endLastHalf = m_pixels.end();
		auto startFirstHalf = m_pixels.begin();
		std::move(startLastHalf, endLastHalf, startFirstHalf);

		m_currRow = m_height;
		m_currOffset = 1;
	}

	inline auto& pixel(size_t x)
	{
		return m_pixels[x + m_currRow*m_width];
	}

private:
	size_t m_maxHeight{ 0 };
	size_t m_currRow{ 0 };
	size_t m_currOffset{ 0 };
	bool m_resize{ true };
};

class PixTextureFlip : public PixTexture
{
public:
	PixTextureFlip(QOpenGLFunctions_3_3_Core* ctx, size_t width, size_t height)
		: PixTexture(ctx, height, width)
	{
	}
	void resize(size_t width, size_t height)
	{
		PixTexture::resize(height, width);
	}
	void nextColumn()
	{
		PixTexture::nextRow();
	}
	inline auto& pixel(size_t y)
	{
		return PixTexture::pixel(y);
	}
};

class PixPlotter : public QOpenGLWidget
{
	Q_OBJECT

public:
	PixPlotter(QWidget* parent);
	//~PixPlotter();
	void initializeGL() override;
	void resizeGL(int width, int height) override;
	void checkState(std::vector<std::vector<double>>& data_out);
	void paintGL() override;

	void drawLabels();

public slots:
	void updateTexture();

private:
	size_t m_fillPos{ 0 };
	size_t m_plotPos{ 0 };
	size_t m_numTimepoints{ 1 };
	size_t m_numFreqpoints{ 1 };
	QOpenGLFunctions_3_3_Core* ctx{ nullptr };
	ShaderProgram* m_sprogram{ nullptr };
	PixTextureFlip* m_pixels{ nullptr };
	GLuint VBO, VAO, EBO;
	std::vector<uint8_t> m_dataStore;

private:
	QTimer* timer;
	DTFTproperties dtft;
	double m_freq = 1;
	static float vertices[];
	static unsigned int indices[];
};

