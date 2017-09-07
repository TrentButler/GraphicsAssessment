#include <gl_core_4_4.h>
#include "Shader.h"
#include <windows.h>
#include <iostream>
#include <fstream>
#include <string>

Shader::Shader() {};
Shader::~Shader() {};

void Shader::bind()
{
	glUseProgram(this->_programID);
}

void Shader::unbind()
{
	glUseProgram(0);
}

void Shader::attach()
{
	int progSuccess = GL_FALSE;

	this->_programID = glCreateProgram();
	glAttachShader(this->_programID, this->vertID);
	glAttachShader(this->_programID, this->fragID);
	glLinkProgram(this->_programID); //AFTER LINKING THE SHADER PROGRAM, CHECK FOR ERRORS
	
	glGetProgramiv(this->_programID, GL_LINK_STATUS, &progSuccess);
	if (progSuccess == GL_FALSE) {
		int infoLogLength = 0;
		glGetProgramiv(this->_programID, GL_INFO_LOG_LENGTH, &infoLogLength);
		char* infoLog = new char[infoLogLength];
		glGetProgramInfoLog(this->_programID, infoLogLength, 0, infoLog);
		printf("SHADER PROGRAM COMPILE ERROR\n");
		printf("%s\n", infoLog);
		delete[] infoLog;
	}

}

void Shader::LoadShader(const char* vert, const char* frag)
{
	/*std::cout << glGetString(GL_SHADING_LANGUAGE_VERSION);
	system("pause");*/

	char previousDir[MAX_PATH];
	char testDir[MAX_PATH];

	const char* shaderDir = "..\\[bin]\\[shaders]";
	GetCurrentDirectory(MAX_PATH, previousDir);

	SetCurrentDirectory(shaderDir);
	GetCurrentDirectory(MAX_PATH, testDir);	

	const char* vertSource = NULL;
	const char* fragSource = NULL;
	std::string tempString = "";
	std::string tmp = "";

	std::fstream vertStream;
	std::fstream fragStream;

#pragma region Vertex
	vertStream.open(vert);
	while (vertStream.eof() == false)
	{
		std::getline(vertStream, tempString);
		tmp += tempString + "\n";
	}
	vertSource = tmp.c_str();
	vertStream.close();
#pragma endregion

	tempString = ""; //CLEAR OUT TEMPORARY STRING
	tmp = ""; //CLEAR OUT TEMPORARY STRING

#pragma region Fragment
	fragStream.open(frag);
	while (fragStream.eof() == false)
	{
		std::getline(fragStream, tempString);
		tmp += tempString + "\n";
	}
	fragSource = tmp.c_str();
	fragStream.close();
#pragma endregion

	//AFTER LOADING SHADER SOURCE CODE, CHECK FOR ERRORS
	int vertSuccess = GL_FALSE;
	int fragSuccess = GL_FALSE;	

	this->vertID = glCreateShader(GL_VERTEX_SHADER); //CREATE AN ID FOR THE VERTEX SHADER
	this->fragID = glCreateShader(GL_FRAGMENT_SHADER); //CREATE AN ID FOR THE FRAGMENT SHADER

	glShaderSource(this->vertID, 1, (const char**)&vertSource, 0);
	glCompileShader(this->vertID); //AFTER COMPILING VERTEX SHADER, CHECK FOR ERRORS
	glGetShaderiv(this->vertID, GL_COMPILE_STATUS, &vertSuccess);
	if (vertSuccess == GL_FALSE) {
		int infoLogLength = 0;
		glGetShaderiv(this->vertID, GL_INFO_LOG_LENGTH, &infoLogLength);
		char* infoLog = new char[infoLogLength];
		glGetShaderInfoLog(this->vertID, infoLogLength, 0, infoLog);
		printf("VERTEX SHADER COMPILE ERROR\n");
		printf("%s\n", infoLog);
		delete[] infoLog;
	}

	glShaderSource(this->fragID, 1, (const char**)&fragSource, 0);
	glCompileShader(this->fragID); //AFTER COMPILING FRAGMENT SHADER, CHECK FOR ERRORS
	glGetShaderiv(this->fragID, GL_COMPILE_STATUS, &fragSuccess);
	if (fragSuccess == GL_FALSE) {
		int infoLogLength = 0;
		glGetShaderiv(this->fragID, GL_INFO_LOG_LENGTH, &infoLogLength);
		char* infoLog = new char[infoLogLength];
		glGetShaderInfoLog(this->fragID, infoLogLength, 0, infoLog);
		printf("FRAGMENT SHADER COMPILE ERROR\n");
		printf("%s\n", infoLog);
		delete[] infoLog;
	}	

	//AFTER COMPILING THE SHADER PROGRAM, DELETE THE INDIVIDUAL SHADERS
	/*glDeleteShader(this->vertID);
	glDeleteShader(this->fragID);*/

	SetCurrentDirectory(previousDir); //RETURN THE WORKING DIRECTORY BACK TO ITS ORIGINAL DIRECTORY
}

unsigned int Shader::getUniform(const char* varName)
{
	return glGetUniformLocation(this->_programID, varName);
}