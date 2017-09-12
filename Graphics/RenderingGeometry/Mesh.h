#pragma once
#include <vector>
#include <vec4.hpp>

struct Vertex
{
	glm::vec4 position;
	glm::vec4 color;
};
class Mesh
{
public:
	Mesh();
	~Mesh();


	void initialize(std::vector<Vertex>& verts, std::vector<unsigned int>& indices);
	void create_buffers();

	void bind(); 
	void unbind();

	void loadOBJ(const char* fileName);

	unsigned int index_count;
	unsigned int vertex_count;

private:
	unsigned int m_vao;
	unsigned int m_vbo;
	unsigned int m_ibo;
	std::vector<Vertex> m_vertices;
	std::vector<unsigned int> m_indices;
};

