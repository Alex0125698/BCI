#pragma once

#include "resources.h"
#include "openglresources.h"

// TODO: use an array of bound + active textures
class ShaderProgram;

struct Pixel
{
	uint8_t r;
	uint8_t g;
	uint8_t b;
	uint8_t a;
};

namespace glw
{
	typedef unsigned int id;

	const uint32_t rgba(const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a);

	// Todo: support for multiple pixel formats (use templates)
	// For now only 8bit/colour supported
	class Texture
	{
	public:
		// basic functions
		Texture(QOpenGLFunctions_3_3_Core* ctx)
			: ctx(ctx)
		{
			if (m_maxTexUnits == 0)
			{
				ctx->glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &m_maxTexUnits);
			}
			m_shader_offset = m_textureCount;
			m_unit = (m_shader_offset % m_maxTexUnits) + GL_TEXTURE0;
			++m_textureCount;

			glwGenTextures();
			glwActiveTexture();
			glwBindTexture();
			ctx->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			ctx->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		}
		Texture(QOpenGLFunctions_3_3_Core* ctx, size_t width, size_t height)
			: Texture(ctx)
		{
			resize(width, height);
			glwTexImage2D();
		}
		Texture(QOpenGLFunctions_3_3_Core* ctx, std::string&& fileName)
			: Texture(ctx)
		{
			loadImage(std::move(fileName));
			glwTexImage2D();
		}
		void resize(size_t width, size_t height)
		{
			m_pixels.resize(width*height);
			m_width = width;
			m_height = height;
		}
		void loadImage(std::string&& fileName);
		constexpr const size_t width() const 
		{
			return m_width;
		}
		constexpr const size_t height() const
		{
			return m_height;
		}
		auto& pixels(size_t x, size_t y)
		{
			return m_pixels[x + y*m_width];
		}
		auto& rawPixels()
		{ 
			return m_pixels; 
		}
	
	public:
		// OpenGL related functions
		void sendToGPU()
		{
			glwActiveTexture();
			glwBindTexture();
			glwTexSubImage2D();
		}
		void generateMipmaps()
		{
			glwActiveTexture();
			glwBindTexture();
			glwGenerateMipmap();
		}

	public:
		// true OpenGL functions

		// @sig GLenum texture
		// Specifies which texture unit to make active.
		// There is an "array" of texture units for each texture type/target
		// You can bind multiple textures of the same target if you use different texture units
		// The number of texture units is implementation dependent, but must be at least 80
		void glwActiveTexture()
		{
			assert(m_glTexId != 0);

			if (m_unit != m_curr_active_unit)
			{
				ctx->glActiveTexture(m_unit);
				m_curr_active_unit = m_unit;
			}
		}

		// @sig GLenum target (1D, 2D etc.), GLuint id
		// associates this texture with the target specified
		// (the previous binding is removed)
		// operations on the target now affect this texture
		void glwBindTexture()
		{
			assert(m_curr_active_unit == m_unit);
			assert(m_glTexId != 0);

			if (m_glTexId != m_curr_bound_tex)
			{
				ctx->glBindTexture(GL_TEXTURE_2D, m_glTexId);
				m_curr_bound_tex = m_glTexId;
			}
		}
		
		// @sig GLsizei count ; GLuint textures[]
		// generates a list of texture names(ids)
		void glwGenTextures()
		{
			assert(m_glTexId == 0);

			ctx->glGenTextures(1, &m_glTexId);
			if (m_glTexId == 0)
				throw DETAILEDEXCEPTION("Texture could not be generated");
		}
		
		// @sig GLsizei count; GLuint textures[]
		// deletes + unbinds an array of textures
		void glwDeleteTextures()
		{
			ctx->glDeleteTextures(1, &m_glTexId);
			m_glTexId = 0;
		}
		
		// @sig GLenum target ; GLint level ; GLint internalFormat ; GLsizei width ; GLsizei height ; 0 ; GLenum format ; GLenum type ; const GLvoid * data
		// level = mipmap level (0 = base image)
		// internalFormat = num color components
		// format = #channels of input data
		// type = bit depth of channels
		// will send the data to the GPU
		void glwTexImage2D()
		{
			assert(m_curr_bound_tex == m_glTexId);
			assert(m_curr_active_unit == m_unit);

			ctx->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid*)&m_pixels[0]);
		}

		// @sig 
		void glwTexSubImage2D()
		{
			assert(m_curr_bound_tex == m_glTexId);
			assert(m_curr_active_unit == m_unit);

			//ctx->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid*)&m_pixels[0]);
			ctx->glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_width, m_height, GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid*)&m_pixels[0]);
		}

		// @sig GLenum target, GLenum pname, GLint param
		void glwTexParameteri()
		{
			float borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };
			ctx->glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

			ctx->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			ctx->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			//GL_DEPTH_STENCIL_TEXTURE_MODE
			//GL_TEXTURE_BASE_LEVEL
			//GL_TEXTURE_COMPARE_FUNC
			//GL_TEXTURE_COMPARE_MODE
			//GL_TEXTURE_LOD_BIAS
			//GL_TEXTURE_MIN_FILTER
			  // GL_NEAREST 
			//GL_TEXTURE_MAG_FILTER
			//GL_TEXTURE_MIN_LOD
			//GL_TEXTURE_MAX_LOD
			//GL_TEXTURE_MAX_LEVEL
			//GL_TEXTURE_SWIZZLE_R
			//GL_TEXTURE_SWIZZLE_G
			//GL_TEXTURE_SWIZZLE_B
			//GL_TEXTURE_SWIZZLE_A
			//GL_TEXTURE_WRAP_S
			//GL_TEXTURE_WRAP_T
			//GL_TEXTURE_WRAP_R

			assert(m_curr_bound_tex == m_glTexId);
			assert(m_curr_active_unit == m_unit);
		}

		// @sig GLenum target
		// generates mipmaps for the bound target
		void glwGenerateMipmap()
		{
			assert(m_curr_bound_tex == m_glTexId);
			assert(m_curr_active_unit == m_unit);

			ctx->glGenerateMipmap(GL_TEXTURE_2D);
		}
		
	private:
		size_t m_width{ 0 };
		size_t m_height{ 0 };
		std::vector<uint32_t> m_pixels;

	private:
		// OpenGL stuff
		QOpenGLFunctions_3_3_Core* ctx;

		GLuint m_glTexId{ 0 };
		// if we change something then we may need to send to GPU again
		bool m_modified{ false };
		// we use the textureCount to generate the offset (in the shaders)
		static id m_textureCount;
		// 
		static GLint m_maxTexUnits;
		// the offset used in the shaders for this texture
		id m_shader_offset;
		//
		id m_unit{ 0 };

		static id m_curr_active_unit;

		static id m_curr_bound_tex;

		friend class ::ShaderProgram;
	};

}