#include "gl_core_4_4.h"
#include "Texture.h"
#include <windows.h>
#include <iostream>
#include <fstream>
#include <string>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <limits>

Texture::Texture() {};
Texture::~Texture() {};

void Texture::load2D(const char* path, const char* name) 
{
	char prevDir[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, prevDir);
	SetCurrentDirectory(path);

	int imageWidth = 0, imageHeight = 0, imageFormat = 0;

	unsigned char* imageData = stbi_load(name, &imageWidth, &imageHeight, &imageFormat, STBI_default); //LOAD IMAGE
	SetCurrentDirectory(prevDir); //RETURN TO ORIGINAL WORKING DIRECTORY

	//GENERATE A OPENGL TEXTURE OBJECT, USE 'm_texture' AS THE HANDLE
	glGenTextures(1, &m_texture);
	glBindTexture(GL_TEXTURE_2D, m_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(imageData);
}

void Texture::generate2D(unsigned int width, unsigned int height, std::vector<float> data)
{
	const int MAX_FLOAT = 250000;

	float* Data = new float[MAX_FLOAT];	

	for (int i = 0; i < data.size(); i++)
	{
		Data[i] = data[i];
	}

	glGenTextures(1, &m_texture); //GENERATE A OPENGL TEXTURE OBJECT, USE 'm_texture' AS THE HANDLE
	glBindTexture(GL_TEXTURE_2D, m_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, width, height, 0, GL_RED, GL_FLOAT, Data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glGenerateMipmap(GL_TEXTURE_2D);

	delete Data;	
	Data = NULL;
}

void Texture::loadCubeMap(const char* path, const char* name)
{

}

void Texture::bind(unsigned int activeTexture, unsigned int textureType) 
{
	glActiveTexture(activeTexture);
	glBindTexture(textureType, m_texture);
}