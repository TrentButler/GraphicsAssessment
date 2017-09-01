#pragma once
#include <vector>

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
	void create_buffers();
	void initialize(std::vector<Vertex> &vertices, std::vector<unsigned int> &indices);
	void bind();
	void unbind();

	unsigned int index_count;
	unsigned int vertex_count;
private:
	unsigned int _vao;
	unsigned int _vbo;
	unsigned int _ibo;

	std::vector<unsigned int> _indices;
	std::vector<Vertex> _vertices;
};