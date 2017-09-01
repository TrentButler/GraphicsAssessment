#include <gl_core_4_4.h>
#include "Shader.h"
#include <windows.h>
#include <iostream>
#include <fstream>
#include <string>

Shader::Shader() {};
Shader::~Shader() {};

void Shader::LoadShader(const char* vert, const char* frag)
{
	/*std::cout << glGetString(GL_SHADING_LANGUAGE_VERSION);
	system("pause");*/

	char previousDir[MAX_PATH];
	const char* shaderDir = "..\\[shaders]";
	GetCurrentDirectory(MAX_PATH, previousDir);
	SetCurrentDirectory(shaderDir);

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
	fragStream.open(vert);
	while (fragStream.eof() == false)
	{
		std::getline(fragStream, tempString);
		tmp += tempString + "\n";
	}
	fragSource = tmp.c_str();
	fragStream.close();
#pragma endregion

	//AFTER LOADING SHADER SOURCE CODE, COMPILE A SHADER PROGRAM
	int vertSuccess = GL_FALSE;
	int fragSuccess = GL_FALSE;
	int progSuccess = GL_FALSE;

	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER); //GET A ID FOR THE VERTEX SHADER
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER); //GET A ID FOR THE FRAGMENT SHADER

	glShaderSource(vertexShader, 1, (const char**)&vertSource, 0);
	glCompileShader(vertexShader); //AFTER COMPILING VERTEX SHADER, CHECK FOR ERRORS
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vertSuccess);
	if (vertSuccess == GL_FALSE) {
		int infoLogLength = 0;
		glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &infoLogLength);
		char* infoLog = new char[infoLogLength];
		glGetShaderInfoLog(vertexShader, infoLogLength, 0, infoLog);
		printf("VERTEX SHADER COMPILE ERROR\n");
		printf("%s\n", infoLog);
		delete[] infoLog;
	}

	glShaderSource(fragmentShader, 1, (const char**)&fragSource, 0);
	glCompileShader(fragmentShader); //AFTER COMPILING FRAGMENT SHADER, CHECK FOR ERRORS
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fragSuccess);
	if (fragSuccess == GL_FALSE) {
		int infoLogLength = 0;
		glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &infoLogLength);
		char* infoLog = new char[infoLogLength];
		glGetShaderInfoLog(fragmentShader, infoLogLength, 0, infoLog);
		printf("FRAGMENT SHADER COMPILE ERROR\n");
		printf("%s\n", infoLog);
		delete[] infoLog;
	}

	this->_programID = glCreateProgram();
	glAttachShader(this->_programID, vertexShader);
	glAttachShader(this->_programID, fragmentShader);
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


	//AFTER COMPILING THE SHADER PROGRAM, DELETE THE INDIVIDUAL SHADERS
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	SetCurrentDirectory(previousDir); //RETURN THE WORKING DIRECTORY BACK TO ITS ORIGINAL DIRECTORY
}

unsigned int Shader::getShaderProgID()
{
	return this->_programID;
}