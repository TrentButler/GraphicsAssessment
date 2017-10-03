#include "gl_core_4_4.h"
#include "Shader.h"
#include <windows.h>
#include <iostream>
#include <fstream>
#include <string>

Shader::Shader() {};
Shader::~Shader() {};

void Shader::bind()
{
	glUseProgram(m_program);
}

void Shader::unbind()
{
	glUseProgram(0);
}

void Shader::load(const char * filename, unsigned int type) 
{
	char prevDir[MAX_PATH];
	const char* shaderDir = "..\\[bin]\\shaders";
	GetCurrentDirectory(MAX_PATH, prevDir);
	SetCurrentDirectory(shaderDir);
	
	std::fstream codeStream;

	std::string tmp = "";
	std::string source = "";

	int vertSuccess = GL_FALSE;
	int fragSuccess = GL_FALSE;

	switch (type)
	{
	case GL_VERTEX_SHADER:
		tmp = "";
		source = "";
		codeStream.open(filename);
		while (codeStream.eof() == false)
		{
			std::getline(codeStream, tmp);
			source += tmp + "\n";
		}
		vsSource = source.c_str();

		m_vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(m_vertexShader, 1, (const char**)&vsSource, 0);
		glCompileShader(m_vertexShader);
		glGetShaderiv(m_vertexShader, GL_COMPILE_STATUS, &vertSuccess);
		if (vertSuccess == GL_FALSE)
		{
			int logLength = 0;
			glGetShaderiv(m_vertexShader, GL_INFO_LOG_LENGTH, &logLength);
			char* infoLog = new char[logLength];
			glGetShaderInfoLog(m_vertexShader, logLength, 0, infoLog);
			printf(filename);
			printf("\n");
			printf("VERTEX SHADER COMPILE ERROR\n");
			printf("%s\n", infoLog);
			delete[] infoLog;
		}
		break;

	case GL_FRAGMENT_SHADER:
		tmp = "";
		source = "";
		codeStream.open(filename);
		while (codeStream.eof() == false)
		{
			std::getline(codeStream, tmp);
			source += tmp + "\n";
		}
		fsSource = source.c_str();
		
		m_fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(m_fragmentShader, 1, (const char**)&fsSource, 0);
		glCompileShader(m_fragmentShader);
		glGetShaderiv(m_fragmentShader, GL_COMPILE_STATUS, &fragSuccess);
		if (fragSuccess == GL_FALSE)
		{
			int logLength = 0;
			glGetShaderiv(m_fragmentShader, GL_INFO_LOG_LENGTH, &logLength);
			char* infoLog = new char[logLength];
			glGetShaderInfoLog(m_fragmentShader, logLength, 0, infoLog);
			printf(filename);
			printf("\n");
			printf("FRAGMENT SHADER COMPILE ERROR\n");
			printf("%s\n", infoLog);
			delete[] infoLog;
		}
		break;
	}

	SetCurrentDirectory(prevDir);
}

void Shader::attach() 
{
	int progSuccess = GL_FALSE;

	m_program = glCreateProgram();
	glAttachShader(m_program, m_vertexShader);
	glAttachShader(m_program, m_fragmentShader);
	glLinkProgram(m_program);

	glGetProgramiv(m_program, GL_LINK_STATUS, &progSuccess);
	if (progSuccess = GL_FALSE)
	{
		int logLength = 0;
		glGetProgramiv(m_program, GL_INFO_LOG_LENGTH, &logLength);
		char* infoLog = new char[logLength];
		glGetProgramInfoLog(m_program, logLength, 0, infoLog);
		printf("SHADER PROGRAM LINK ERROR\n");
		printf("%s\n", infoLog);
		delete[] infoLog;
	}
}

void Shader::defaultLoad()
{
	// create shaders
	vsSource = "#version 410\n \
	layout(location=0) in vec4 position; \
	layout(location=1) in vec4 colour; \
	out vec4 vColour; \
	uniform mat4 projectionViewWorldMatrix; \
	void main() { vColour = colour; gl_Position = \
	projectionViewWorldMatrix * position; \
	";

	fsSource = "#version 410\n \
	in vec4 vColour; \
	out vec4 fragColor; \
	void main() { fragColor = vColour; }";

	int success = GL_FALSE;
	unsigned int m_vertexShader = glCreateShader(GL_VERTEX_SHADER);
	unsigned int m_fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	
	glShaderSource(m_vertexShader, 1, (const char**)&vsSource, 0);
	glCompileShader(m_vertexShader);
	glShaderSource(m_fragmentShader, 1, (const char**)&fsSource, 0);
	glCompileShader(m_fragmentShader);

	m_program = glCreateProgram();
	glAttachShader(m_program, m_vertexShader);
	glAttachShader(m_program, m_fragmentShader);
	glLinkProgram(m_program);
	glGetProgramiv(m_program, GL_LINK_STATUS, &success);
	if (success == GL_FALSE) {
		int infoLogLength = 0;
		glGetProgramiv(m_program, GL_INFO_LOG_LENGTH, &infoLogLength);
		char* infoLog = new char[infoLogLength];
		glGetProgramInfoLog(m_program, infoLogLength, 0, infoLog);
		printf("Error: Failed to link shader program!\n");
		printf("%s\n", infoLog);
		delete[] infoLog;
	}

	glDeleteShader(m_fragmentShader);
	glDeleteShader(m_vertexShader);
}

//void load();

unsigned int Shader::getUniform(const char * name) 
{
	return glGetUniformLocation(m_program, name);
}