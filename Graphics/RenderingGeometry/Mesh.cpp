#include <gl_core_4_4.h>
#include "Mesh.h"
#include <iostream>
#include <fstream>
#include <string>

Mesh::Mesh() {};
Mesh::~Mesh() {};


void Mesh::initialize(std::vector<Vertex>& verts, std::vector<unsigned int>& indices) 
{
	for (auto v : verts)
	{
		m_vertices.push_back(v);
	}

	for (auto i : indices)
	{
		m_indices.push_back(i);
	}

	index_count = m_indices.size();
	vertex_count = m_vertices.size();

	create_buffers();
}

void Mesh::create_buffers() 
{
	//GENERATE
	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_vbo);
	glGenBuffers(1, &m_ibo);
	
	//BIND
	glBindVertexArray(m_vao);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, vertex_count * sizeof(Vertex), m_vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_count * sizeof(unsigned int), m_indices.data(), GL_STATIC_DRAW);
	
	//GIVE INFO OF BUFFERS TO OPENGL
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)sizeof(glm::vec4));

	//CLEAN UP
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void Mesh::bind() 
{
	glBindVertexArray(m_vao);
}

void Mesh::unbind() 
{
	glBindVertexArray(0);
}

void Mesh::loadOBJ(const char* fileName)
{
	//CHANGE THE WORKING DIR TO "..//[bin]//objects"
	//PARSE FILE
	//	- IF LINE STARTS WITH 'v' ADD TO THE 'VERTICES' ARRAY
	//	- IF LINE STARTS WITH 'f' ADD TO THE 'INDICES' ARRAY

	char prevDir[MAX_PATH];
	const char* newDir = "..//[bin]//objects";
	GetCurrentDirectory(MAX_PATH, prevDir);
	SetCurrentDirectory(newDir);

	std::string line = "";
	std::string tmp = "";

	std::fstream objStream;
	objStream.open(fileName);
	while (objStream.eof() == false)
	{
		std::getline(objStream, line);

		if (line[0] == 'v')
		{
			int value = 0;
			std::string x = "";
			std::string y = "";
			std::string z = "";
			glm::vec4 vert = glm::vec4(0);
			for (int i = 3; i < line.size(); i++)
			{
				if (line[i] == ' ')
				{
					//NEXT VALUE
					value++;
				}

				switch (value)
				{
				case 0:
				{
					x += line[i];
					break;
				}

				case 1:
				{
					y += line[i];
					break;
				}
				
				case 2:
				{
					z += line[i];
					break;
				}
				}
			}

			//CREATE A VEC4
			auto X = std::stof(x);
			auto Y = std::stof(y);
			auto Z = std::stof(z);
			//GENERATE A VERTEX

			glm::vec4 vertex = glm::vec4(X, Y, Z, 1.0f);
			//m_vertices.push_back(vertex);
			//CHECK IF THE XYZ VALUE IS NEGATIVE (-)
			//ONCE A SPACE IS FOUND (' ') POPULATE THE NEXT VALUE
		}
	}


	SetCurrentDirectory(prevDir);
}