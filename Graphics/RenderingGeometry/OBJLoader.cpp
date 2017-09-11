#include "OBJLoader.h"
#include <iostream>
#include <fstream>
#include <string>

OBJLoader::OBJLoader() {};
OBJLoader::~OBJLoader() {};

void OBJLoader::loadOBJ(const char* fileName, std::vector<Vertex> &vertices, std::vector<unsigned int> &indices) 
{
	//PARSE FILE
	//	- IF LINE STARTS WITH 'v' ADD TO THE 'VERTICES' ARRAY
	//	- IF LINE STARTS WITH 'f' ADD TO THE 'INDICES' ARRAY

	std::vector<Vertex> verts;
	std::vector<unsigned int> indes;	
	std::string line = "";
	std::string tmp = "";

	std::fstream objStream;
	objStream.open(fileName);
	while (objStream.eof() == false)
	{
		std::getline(objStream, line);

		if (line[0] == 'v')
		{

		}
	}


}