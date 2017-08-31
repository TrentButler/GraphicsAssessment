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
	char previousDir[MAX_PATH];
	const char* shaderDir = "..\\Release\\[shaders]";	
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
	int success = GL_FALSE;
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	const char * vsSource = "#version 410\n \
                            layout(location = 0) in vec4 Position; \
                            layout(location = 1) in vec4 Color; \
                            out vec4 vColor; \
                            uniform mat4 ProjectionViewWorld; \
                            void main() { vColor = Color; \
                            gl_Position = ProjectionViewWorld * Position; }";

	const char * fSource = "#version 410\n \
                            in vec4 vColor; \
                            out vec4 FragColor; \
                            void main() { FragColor = vColor; }";


	glShaderSource(vertexShader, 1, (const char**)&vsSource, 0);
	glCompileShader(vertexShader);

	glShaderSource(fragmentShader, 1, (const char**)&fSource, 0);
	glCompileShader(fragmentShader);

	this->_programID = glCreateProgram();
	glAttachShader(this->_programID, vertexShader);
	glAttachShader(this->_programID, fragmentShader);
	glLinkProgram(this->_programID);

	//AFTER LINKING THE SHADER PROGRAM, CHECK FOR ERRORS
	glGetProgramiv(this->_programID, GL_LINK_STATUS, &success);
	if (success == GL_FALSE) {
		int infoLogLength = 0;
		glGetProgramiv(this->_programID, GL_INFO_LOG_LENGTH, &infoLogLength);
		char* infoLog = new char[infoLogLength];
		glGetProgramInfoLog(this->_programID, infoLogLength, 0, infoLog);
		printf("Error: Failed to link shader program!\n");
		printf("%s\n", infoLog);
		delete[] infoLog;
	}

	//IF LINKING IS SUCCESSFUL, DELETE THE INDIVIDUAL SHADERS
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	SetCurrentDirectory(previousDir); //RETURN THE WORKING DIRECTORY BACK TO ITS ORIGINAL DIRECTORY
}

unsigned int Shader::getShaderProgID() 
{
	return this->_programID;
}