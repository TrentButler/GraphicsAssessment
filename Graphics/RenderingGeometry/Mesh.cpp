#include <gl_core_4_4.h>
#include <gtc/type_ptr.hpp>
#include "Mesh.h"

Mesh::Mesh() {};
Mesh::~Mesh() {};

//void Mesh::create_buffers() 
//{
//	glGenVertexArrays(1, &this->_vao); //GENRATE THE VERTEX ARRAY OBJECT BEFORE THE VBO AND IBO
//
//	glGenBuffers(1, &this->_vbo); //GENERATE A VBO
//	glGenBuffers(1, &this->_ibo); //GENERATE A IBO
//	glBindVertexArray(this->_vao);
//
//	glBindBuffer(GL_ARRAY_BUFFER, this->_vbo); //BIND THE VBO
//	glBufferData(GL_ARRAY_BUFFER, this->vertex_count * sizeof(Vertex), glm::value_ptr(_vertices), GL_STATIC_DRAW); //SEND OPENGL THE VBO'S INFORMATION
//
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->_ibo); //BIND THE IBO
//	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
//		this->index_count * sizeof(unsigned int), glm::value_ptr(_indices), GL_STATIC_DRAW); //SEND OPENGL THE IBO'S INFORMATION
//
//	glEnableVertexAttribArray(0);
//	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0); //DEFINE THE SIZE OF THE VAO
//	glEnableVertexAttribArray(1);
//	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)sizeof(glm::vec4)); //DEFINE THE SIZE OF THE VAO
//
//	glBindVertexArray(0); //UNBIND THE VAO
//	glBindBuffer(GL_ARRAY_BUFFER, 0); //UNBIND THE VBO
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); //UNBIND THE IBO
//
//};

void Mesh::initialize(std::vector<Vertex> &vertices, std::vector<unsigned int> &indices) 
{
	for (int i = 0; i < vertices.size(); i++)
	{
		this->_vertices.push_back(vertices[i]); //POPULATE VERTEX ARRAY
	}
	this->vertex_count = this->_vertices.size(); //STORE THE SIZE AFTER THE ARRAY POPULATION 

	for (int i = 0; i < indices.size(); i++)
	{
		this->_indices.push_back(indices[i]); //POPULATE INDEX ARRAY
	}
	this->index_count = this->_indices.size(); //STORE THE SIZE AFTER THE ARRAY POPULATION

}

void Mesh::bind() 
{
	glBindBuffer(GL_ARRAY_BUFFER, this->_vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->_ibo);
}

void Mesh::unbind() 
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}