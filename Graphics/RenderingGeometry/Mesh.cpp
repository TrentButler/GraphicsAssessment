#include <gl_core_4_4.h>
#include "Mesh.h"

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