#include "texture.h"
#include <codeanalysis\warnings.h>
#pragma warning(push, 0)
#pragma warning ( disable : ALL_CODE_ANALYSIS_WARNINGS )
#include "stb_image.h"
#pragma warning(pop)

glw::id glw::basic::Texture::m_textureCount{ 0 };
GLint glw::basic::Texture::m_maxTexUnits{ 0 };

void glw::Texture::loadImage(std::string&& fileName)
{
	int nrChannels = 0;
	stbi_set_flip_vertically_on_load(true);
	auto data = stbi_load(fileName.c_str(), (int*)&m_width, (int*)&m_height, &nrChannels, 0);

	if (data == nullptr)
		throw DETAILEDEXCEPTION(QString::fromStdString("failed to load image: " + fileName));

	m_pixels.resize(m_width*m_height);

	if (nrChannels == 4)
	{
		std::copy((uint32_t*)data, (uint32_t*)data + m_width*m_height, m_pixels.begin());
	}
	else if (nrChannels == 3)
	{
		for (size_t i = 0; i < m_pixels.size(); ++i)
			m_pixels[i] = (data[i*3 + 2] << 16u) | (data[i*3 + 1] << 8u) | data[i*3];
	}
	else
		throw DETAILEDEXCEPTION(QString::fromStdString("Unsupported number of channels: " + std::to_string(nrChannels)));

	basic.glwActiveTexture();
	basic.glwBindTexture();

	stbi_image_free(data);
}

