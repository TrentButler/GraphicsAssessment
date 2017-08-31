#pragma once
#include <Application.h>
#include <Camera.h>
#include "Shader.h"
#include <glm.hpp>
#include <vector>
struct GLFWwindow;
struct GLFWmonitor;
struct Vertex
{
	glm::vec4 position;
	glm::vec4 color;
};
class RenderApp : public Application
{
public:
	RenderApp();
	~RenderApp();

	void startup();
	void shutdown();
	void update(float deltaTime);
	void draw();

	void genGrid(unsigned int rows, unsigned int cols);	

private:
	

	unsigned int _vao;
	unsigned int _vbo;
	unsigned int _ibo;

	unsigned int m_vao;
	unsigned int m_ibo;
	unsigned int m_vbo;

	std::vector<unsigned int> m_indices;
	std::vector<Vertex> m_vertices;


	int _rows;
	int _cols;

	Camera* _camera;
	Shader* _shader;
};