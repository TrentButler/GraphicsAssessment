#include <gl_core_4_4.h>
#include "Texture.h"
#include <windows.h>
#include <iostream>
#include <fstream>
#include <string>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

Texture::Texture() {};
Texture::~Texture() {};

void Texture::load(const char* path, const char* name) 
{
	char prevDir[MAX_PATH];
	char testDir[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, prevDir);
	SetCurrentDirectory(path);
	GetCurrentDirectory(MAX_PATH, testDir);
	std::cout << testDir;

	int imageWidth = 0, imageHeight = 0, imageFormat = 0;

	unsigned char* imageData = stbi_load(name, &imageWidth, &imageHeight, &imageFormat, STBI_default); //LOAD IMAGE
	SetCurrentDirectory(prevDir); //RETURN TO ORIGINAL WORKING DIRECTORY

	//GENERATE A OPENGL TEXTURE OBJECT, USE 'm_texture' AS THE HANDLE
	glGenTextures(1, &m_texture);
	glBindTexture(GL_TEXTURE_2D, m_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	stbi_image_free(imageData);
}

void Texture::bind() 
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_texture);
}