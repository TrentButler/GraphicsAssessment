#include "RenderingGeometryApp.h"
#include "Shader.h"
#include "Mesh.h"
#include <glfw3.h>
#include <FlyCamera.h>
#include <vector>
#include <vec4.hpp>
#include <ext.hpp>
#include <gtc/constants.hpp>


#pragma region 1.Function that generates a half circle given a number of points and radius.

#pragma endregion

#pragma region 2.Function that generates a sphere given a half circle, and number of meridians.

#pragma endregion

#pragma region 3.Function that generates indices for geometry to be rendered using triangle strips.

#pragma endregion

#pragma region 4.Ability to render a plane with predefined vertex information.

#pragma endregion

#pragma region 5.Ability to render a cube with predefined vertex information.

Mesh* generateCube()
{
	Mesh* Cube = new Mesh();

	Vertex a = { glm::vec4(0.0f,0.0f,-2.0f,1.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), glm::vec4(0), glm::vec3(0) }; // BOTTOM LEFT
	Vertex b = { glm::vec4(1.0f,0.0f,-2.0f,1.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), glm::vec4(0), glm::vec3(0) }; // BOTTOM RIGHT
	Vertex c = { glm::vec4(1.0f,1.0f,-2.0f,1.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), glm::vec4(0), glm::vec3(0) }; // TOP RIGHT
	Vertex d = { glm::vec4(0.0f,1.0f,-2.0f,1.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), glm::vec4(0), glm::vec3(0) }; // TOP LEFT
	Vertex e = { glm::vec4(0.0f,0.0f,2.0f,1.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), glm::vec4(0), glm::vec3(0) }; // BACK BOTTOM LEFT
	Vertex f = { glm::vec4(1.0f,0.0f,2.0f,1.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), glm::vec4(0), glm::vec3(0) }; // BACK BACK BOTTOM RIGHT
	Vertex g = { glm::vec4(1.0f,1.0f,2.0f,1.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), glm::vec4(0), glm::vec3(0) }; // BACK TOP RIGHT
	Vertex h = { glm::vec4(0.0f,1.0f,2.0f,1.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), glm::vec4(0), glm::vec3(0) }; // BACK TOP LEFT

	std::vector<Vertex> verts = { a, b, c, d, e, f, g, h };
	std::vector<unsigned int> indes = { 
		0,1,3,
		3,1,2, //FRONT FACE

		4,5,7,
		7,5,6, //BACK FACE
		
		4,0,7,
		7,0,3, //LEFT FACE

		1,5,2,
		2,5,6, //RIGHT FACE

		3,2,7,
		7,2,6, //TOP FACE

		0,1,4,
		4,1,5  //BOTTOM FACE
	};

	Cube->initialize(verts, indes);
	return Cube;
}

#pragma endregion

#pragma region 6.Ability to render a sphere with Triangles

#pragma endregion

#pragma region 7.Ability to render the procedurally generated sphere with triangle strips.

#pragma endregion

#pragma region 8.Ability to load shaders from file using a Shader class object.

#pragma endregion

#pragma region 9.Ability to load geometry into a Mesh class object.

#pragma endregion


RenderingGeometryApp::RenderingGeometryApp() {};
RenderingGeometryApp::~RenderingGeometryApp() {};

void RenderingGeometryApp::startup()
{

	m_camera = new FlyCamera();
	m_camera->setLookAt(glm::vec3(10, 10, 10), glm::vec3(0), glm::vec3(0, 1, 0));
	m_camera->setPerspective(glm::pi<float>() / 4, (float)_width / (float)_height, 0.1f, 1000.0f);

	m_shader = new Shader();
	m_shader->load("triShader.vert", GL_VERTEX_SHADER);
	m_shader->load("triShader.frag", GL_FRAGMENT_SHADER);
	m_shader->attach();

	Vertex a = { glm::vec4(5.0f, 0.0f, 0.0f, 1.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f) };
	Vertex b = { glm::vec4(0.0f, 5.0f, 0.0f, 1.0f), glm::vec4(0.0f, 1.0f, 0.0f, 1.0f) };
	Vertex c = { glm::vec4(-5.0f, 0.0f, 0.0f, 1.0f), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f) };

	std::vector<Vertex> triVerts = {a, b, c};
	std::vector<unsigned int> triIndices = { 0, 1, 2 };

	m_object = generateCube();
	//m_object->initialize(triVerts, triIndices);

}

void RenderingGeometryApp::shutdown() {}

void RenderingGeometryApp::update(float time) 
{
	//CAMERA STUFF

}

void RenderingGeometryApp::draw()
{
	m_shader->bind();
	
	unsigned int projectionviewUniform = m_shader->getUniform("worldViewProjection");

	auto projView = m_camera->getProjectionView();

	glUniformMatrix4fv(projectionviewUniform, 1, GL_FALSE, glm::value_ptr(projView));

	m_object->bind();

	glDrawElements(GL_TRIANGLES, m_object->index_count, GL_UNSIGNED_INT, 0);

	m_object->unbind();
	m_shader->unbind();

}
