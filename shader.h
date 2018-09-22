#pragma once

#include "openglresources.h"
#include "resources.h"
#include "texture.h"

typedef unsigned int GLuint;

class Shader
{
public:
	enum class Type
	{
		VERTEX,
		GEOMETRY,
		FRAGMENT
	};

public:
	Shader(QOpenGLFunctions_3_3_Core* ctx, Type shaderType);
	Shader(QOpenGLFunctions_3_3_Core* ctx, Type shaderType, const std::string& sourceFile);
	Shader(QOpenGLFunctions_3_3_Core* ctx, Type shaderType, const std::string&& sourceFile);
	void compileSource(const std::string& source);
	void compileSource(const std::string&& source);
	void compileSourceFile(const std::string& fileName);
	void compileSourceFile(const std::string&& fileName);
	virtual ~Shader() = 0;

private:
	const Type m_shader_type;
	GLuint m_shader_id{ 0 };
	QOpenGLFunctions_3_3_Core* ctx;

	friend class ShaderProgram;
};

class VertexShader : public Shader
{
public:
	VertexShader(QOpenGLFunctions_3_3_Core* ctx) noexcept : Shader{ctx,  Type::VERTEX } {}
	explicit VertexShader(QOpenGLFunctions_3_3_Core* ctx, const std::string& source) : Shader{ctx,  Type::VERTEX, source } {}
	explicit VertexShader(QOpenGLFunctions_3_3_Core* ctx, const std::string&& source) : Shader{ctx,  Type::VERTEX, source } {}
};

class GeometryShader : public Shader
{
public:
	GeometryShader(QOpenGLFunctions_3_3_Core* ctx) noexcept : Shader{ctx, Type::GEOMETRY } {}
	explicit GeometryShader(QOpenGLFunctions_3_3_Core* ctx, const std::string& source) : Shader{ctx, Type::GEOMETRY, source } {}
	explicit GeometryShader(QOpenGLFunctions_3_3_Core* ctx, const std::string&& source) : Shader{ctx, Type::GEOMETRY, source } {}
};

class FragmentShader : public Shader
{
public:
	FragmentShader(QOpenGLFunctions_3_3_Core* ctx) noexcept : Shader{ctx, Type::FRAGMENT } {}
	explicit FragmentShader(QOpenGLFunctions_3_3_Core* ctx, const std::string& source) : Shader{ctx, Type::FRAGMENT, source } {}
	explicit FragmentShader(QOpenGLFunctions_3_3_Core* ctx, const std::string&& source) : Shader{ctx, Type::FRAGMENT, source } {}
};

class ShaderProgram
{
public:
	ShaderProgram(QOpenGLFunctions_3_3_Core* ctx)
		: ctx(ctx)
	{
	}
	~ShaderProgram();
	void compile();
	void setVertexShader(std::shared_ptr<VertexShader> shader) { m_vshader = shader; }
	void setGeometryShader(std::shared_ptr<GeometryShader> shader) { m_gshader = shader; }
	void setFragmentShader(std::shared_ptr<FragmentShader> shader) { m_fshader = shader; }
	void setActive();
	void setTextureLocation(const char uniformTexName[], const glw::Texture& tex)
	{
		ctx->glUniform1i(ctx->glGetUniformLocation(m_program_id, uniformTexName), (tex.basic.m_unit - GL_TEXTURE0));
	}
	GLuint m_program_id{ 0 };
private:
	std::shared_ptr<VertexShader> m_vshader;
	std::shared_ptr<GeometryShader> m_gshader;
	std::shared_ptr<FragmentShader> m_fshader;
	QOpenGLFunctions_3_3_Core* ctx;
};

