#include "resources.h"
#include <fstream>
#include <sstream>
#include "shader.h"

// ===== Shader =====

Shader::Shader(QOpenGLFunctions_3_3_Core* ctx, Type shaderType)
	: m_shader_type{ shaderType }, ctx{ ctx }
{
}

Shader::Shader(QOpenGLFunctions_3_3_Core* ctx, Type shaderType, const std::string& sourceFile)
	: Shader{ctx, shaderType }
{
	compileSourceFile(sourceFile);
}

Shader::Shader(QOpenGLFunctions_3_3_Core* ctx, Type shaderType, const std::string&& sourceFile)
	: Shader{ctx, shaderType }
{
	compileSourceFile(std::move(sourceFile));
}

void Shader::compileSource(const std::string& source)
{
	if (m_shader_type == Type::VERTEX)
		m_shader_id = ctx->glCreateShader(GL_VERTEX_SHADER);
	else if (m_shader_type == Type::GEOMETRY)
		m_shader_id = ctx->glCreateShader(GL_GEOMETRY_SHADER);
	else
		m_shader_id = ctx->glCreateShader(GL_FRAGMENT_SHADER);

	// this is needed to 'create' an array of cstrings
	// although we only have 1 string
	const char* const cstr = source.c_str();

	// copies the source string into the shader object
	// args => {shader id ; 1 source string ; cstring array ; null terminated}
	ctx->glShaderSource(m_shader_id, 1, &cstr, nullptr);
	// compiles the shader source
	ctx->glCompileShader(m_shader_id);

	// check if compilation was successful
	GLint status;
	ctx->glGetShaderiv(m_shader_id, GL_COMPILE_STATUS, &status);
	if (status != GL_TRUE)
	{
		// create exception with compile log
		char buff[512];
		ctx->glGetShaderInfoLog(m_shader_id, 512, nullptr, buff);
		// TODO: throw exception
		throw 0;
	}
}

void Shader::compileSource(const std::string&& source)
{
	compileSource(source);
}

void Shader::compileSourceFile(const std::string& fileName)
{
	std::ifstream in;
	//in.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	in.open(fileName);

	if (!in.is_open())
	{
		throw DETAILEDEXCEPTION(QString::fromStdString("file: " + fileName + " could not be opened"));
	}

	// apparently this is the fastest way to get the file
	std::string str(static_cast<std::stringstream const&>(std::stringstream() << in.rdbuf()).str());
	compileSource(str);
}

void Shader::compileSourceFile(const std::string&& fileName)
{
	compileSourceFile(fileName);
}

Shader::~Shader()
{
	// the shader object files are no longer needed after the
	// final shader program has been created
	if (m_shader_id != 0)
	{
		ctx->glDeleteShader(m_shader_id);
		m_shader_id = 0;
	}
}

// ===== ShaderProgram =====

ShaderProgram::~ShaderProgram()
{
	// detach
}

void ShaderProgram::compile()
{
	// check that we have the required shaders
	if (m_vshader == nullptr)
		throw 0;
	if (m_fshader == nullptr)
		throw 0;

	m_program_id = ctx->glCreateProgram();

	// attach shader objects to program
	ctx->glAttachShader(m_program_id, m_vshader->m_shader_id);
	if (m_gshader != nullptr)
		ctx->glAttachShader(m_program_id, m_gshader->m_shader_id);
	ctx->glAttachShader(m_program_id, m_fshader->m_shader_id);

	// this creates the executable for the vertex processor
	ctx->glLinkProgram(m_program_id);

	// check whether there are linking errors
	GLint status;
	ctx->glGetProgramiv(m_program_id, GL_LINK_STATUS, &status);
	if (status != GL_TRUE)
	{
		// create exception with link status
		// TODO: get these buffers off of stack
		char buff[512];
		ctx->glGetProgramInfoLog(m_program_id, 512, nullptr, buff);
		throw 0;
	}
}

void ShaderProgram::setActive()
{
	ctx->glUseProgram(m_program_id);
}
