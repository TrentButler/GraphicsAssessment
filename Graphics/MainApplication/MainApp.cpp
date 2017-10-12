#include "MainApp.h"
#include "SceneManager.h"
#include <glfw3.h>

#include <Mesh.h>
#include <Shader.h>
#include <Texture.h>
#include <FlyCamera.h>

#include <ext.hpp>
#include <gtc\constants.hpp>
#include <mat4x4.hpp>
#include <vec4.hpp>

#include <vector>
#include <string.h>

#pragma region GenerateSphere
Mesh* generateSphere(unsigned int segments, unsigned int rings,
	unsigned int& vao, unsigned int& vbo, unsigned int& ibo,
	unsigned int& indexCount) {

	unsigned int vertCount = (segments + 1) * (rings + 2);
	indexCount = segments * (rings + 1) * 6;

	// using AIEVertex for now, but could be any struct as long as it has the correct elements
	Vertex* vertices = new Vertex[vertCount];
	unsigned int* indices = new unsigned int[indexCount];

	float ringAngle = glm::pi<float>() / (rings + 1);
	float segmentAngle = 2.0f * glm::pi<float>() / segments;

	Vertex* vertex = vertices;

	for (unsigned int ring = 0; ring < (rings + 2); ++ring) {
		float r0 = glm::sin(ring * ringAngle);
		float y0 = glm::cos(ring * ringAngle);

		for (unsigned int segment = 0; segment < (segments + 1); ++segment, ++vertex) {
			float x0 = r0 * glm::sin(segment * segmentAngle);
			float z0 = r0 * glm::cos(segment * segmentAngle);

			vertex->position = glm::vec4(x0 * 0.5f, y0 * 0.5f, z0 * 0.5f, 1);
			vertex->normal = glm::vec4(x0, y0, z0, 0);

			vertex->tangent = glm::vec4(glm::sin(segment * segmentAngle + glm::half_pi<float>()), 0, glm::cos(segment * segmentAngle + glm::half_pi<float>()), 0);

			//// not a part of the AIEVertex, but this is how w generate bitangents
			//vertex->bitangent = glm::vec4(glm::cross(glm::vec3(vertex->normal), glm::vec3(vertex->tangent)), 0);

			vertex->texture = glm::vec3(segment / (float)segments, ring / (float)(rings + 1), 0);
		}
	}

	unsigned int index = 0;
	for (unsigned i = 0; i < (rings + 1); ++i) {
		for (unsigned j = 0; j < segments; ++j) {
			indices[index++] = i * (segments + 1) + j;
			indices[index++] = (i + 1) * (segments + 1) + j;
			indices[index++] = i * (segments + 1) + (j + 1);

			indices[index++] = (i + 1) * (segments + 1) + (j + 1);
			indices[index++] = i * (segments + 1) + (j + 1);
			indices[index++] = (i + 1) * (segments + 1) + j;
		}
	}

	Mesh* sphere = new Mesh();
	std::vector<Vertex> verts;
	std::vector<unsigned int> indes;
	for (int i = 0; i < vertCount; i++)
	{
		verts.push_back(vertices[i]);
	}
	for (int j = 0; j < indexCount; j++)
	{
		indes.push_back(indices[j]);
	}
	sphere->initialize(verts, indes);
	return sphere;

	// generate buffers
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ibo);

	// generate vertex array object (descriptors)
	glGenVertexArrays(1, &vao);

	// all changes will apply to this handle
	glBindVertexArray(vao);

	// set vertex buffer data
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertCount * sizeof(Vertex), vertices, GL_STATIC_DRAW);

	// index data
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(unsigned int), indices, GL_STATIC_DRAW);

	// position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

	// colors
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(glm::vec4)));
	// normals
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_TRUE, sizeof(Vertex), (void*)(sizeof(glm::vec4) * 2));

	// texcoords
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(glm::vec4) * 3));

	// tangents
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_TRUE, sizeof(Vertex), (void*)(sizeof(glm::vec4) * 3 + sizeof(glm::vec2)));

	// safety
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	delete[] indices;
	delete[] vertices;
}
#pragma endregion


MainApp::MainApp() {}
MainApp::~MainApp() {}

void MainApp::startup() 
{
	m_camera = new FlyCamera();
	//m_camera->setLookAt(glm::vec3(-20.1f, 50.0f, -100.1f), glm::vec3(0), glm::vec3(0, 1, 0));
	m_camera->setLookAt(glm::vec3(-20.1f, 50.0f, -150.1f), glm::vec3(150, 0, 0), glm::vec3(0, 1, 0));
	m_camera->setPerspective(glm::pi<float>() / 4, (float)_width / (float)_height, 0.1f, 10000.0f);

	unsigned int vao, vbo, ibo, indexcount;
	m_sphere = generateSphere(100, 100, vao, vbo, ibo, indexcount);

	m_shader = new Shader();
	m_shader->load("textureShader.vert", GL_VERTEX_SHADER);
	m_shader->load("textureShader.frag", GL_FRAGMENT_SHADER);
	m_shader->attach();

	m_animatedTexture = new Texture();
	m_animatedTexture->load2D("..//[bin]//textures//diffuse", "starfieldDiffuseMap.jpg");
}

void MainApp::shutdown() {}

glm::mat4 skyboxTransform = glm::mat4(1);
float runningTime = 0;
void MainApp::update(float deltaTime)
{
	runningTime += deltaTime;

	skyboxTransform = glm::scale(glm::vec3(1000));
	//CAMERA STUFF	
	if (glfwGetMouseButton(Application::_window, 0) == true) //LOOK AT CENTER (0,0,0)
	{
		m_camera->setLookAt(m_camera->getWorldTransform()[3], glm::vec4(0), glm::vec3(0, 1, 0));
	}

	if (glfwGetKey(Application::_window, GLFW_KEY_W))
	{
		//MOVE FOWARD (-Z)
		//GET PREVIOUS POSITION, TRANSLATE, THEN SET IT
		auto Previous = m_camera->getWorldTransform();
		auto Translation = glm::mat4(
			glm::vec4(1, 0, 0, 0),
			glm::vec4(0, 1, 0, 0),
			glm::vec4(0, 0, 1, 0),
			glm::vec4(0, 0, -100 * deltaTime, 1));

		//auto newPositon = (Previous * Translation)[3];
		//this->_camera->setPosition(newPositon);
		auto newWorld = Previous * Translation;
		m_camera->setWorldTransform(newWorld);
	}

	if (glfwGetKey(Application::_window, GLFW_KEY_S))
	{
		//MOVE BACKWARD (+Z)
		auto Previous = m_camera->getWorldTransform();
		auto Translation = glm::mat4(glm::vec4(1, 0, 0, 0), glm::vec4(0, 1, 0, 0),
			glm::vec4(0, 0, 1, 0), glm::vec4(0, 0, 100 * deltaTime, 1));

		//auto newPositon = (Previous * Translation)[3];
		//this->_camera->setPosition(newPositon);
		auto newWorld = Previous * Translation;
		m_camera->setWorldTransform(newWorld);
	}

	if (glfwGetKey(Application::_window, GLFW_KEY_A))
	{
		//MOVE LEFT (-X)
		auto Previous = m_camera->getWorldTransform();
		auto Translation = glm::mat4(glm::vec4(1, 0, 0, 0), glm::vec4(0, 1, 0, 0),
			glm::vec4(0, 0, 1, 0), glm::vec4(-100 * deltaTime, 0, 0, 1));

		//auto newPositon = (Previous * Translation)[3];
		//this->_camera->setPosition(newPositon);
		auto newWorld = Previous * Translation;
		m_camera->setWorldTransform(newWorld);
	}

	if (glfwGetKey(Application::_window, GLFW_KEY_D))
	{
		//MOVE RIGHT (+X)
		auto Previous = m_camera->getWorldTransform();
		auto Translation = glm::mat4(glm::vec4(1, 0, 0, 0), glm::vec4(0, 1, 0, 0),
			glm::vec4(0, 0, 1, 0), glm::vec4(100 * deltaTime, 0, 0, 1));

		//auto newPositon = (Previous * Translation)[3];
		//this->_camera->setPosition(newPositon);
		auto newWorld = Previous * Translation;
		m_camera->setWorldTransform(newWorld);
	}

	if (glfwGetKey(Application::_window, GLFW_KEY_Q))
	{
		//MOVE UP (+Y)
		auto Previous = m_camera->getWorldTransform();
		auto Translation = glm::mat4(
			glm::vec4(1, 0, 0, 0),
			glm::vec4(0, 1, 0, 0),
			glm::vec4(0, 0, 1, 0),
			glm::vec4(0, 100 * deltaTime, 0, 1));

		//auto newPositon = (Previous * Translation)[3];
		//this->_camera->setPosition(newPositon);
		auto newWorld = Previous * Translation;
		m_camera->setWorldTransform(newWorld);
	}

	if (glfwGetKey(Application::_window, GLFW_KEY_E))
	{
		//MOVE DOWN (-Y)
		auto Previous = m_camera->getWorldTransform();
		auto Translation = glm::mat4(
			glm::vec4(1, 0, 0, 0),
			glm::vec4(0, 1, 0, 0),
			glm::vec4(0, 0, 1, 0),
			glm::vec4(0, -100 * deltaTime, 0, 1));

		//auto newPositon = (Previous * Translation)[3];
		//this->_camera->setPosition(newPositon);
		auto newWorld = Previous * Translation;
		m_camera->setWorldTransform(newWorld);
	}

	if (glfwGetKey(Application::_window, GLFW_KEY_R))
	{
		system("cls");
		m_shader->load("textureShader.vert", GL_VERTEX_SHADER);
		m_shader->load("textureShader.frag", GL_FRAGMENT_SHADER);
		m_shader->attach();
	}

	if (glfwGetKey(Application::_window, GLFW_KEY_KP_0))
	{
		m_camera->setLookAt(m_camera->getWorldTransform()[3], skyboxTransform[3], glm::vec3(0, 1, 0));
	}

	if (glfwGetKey(Application::_window, GLFW_KEY_ESCAPE))
	{
		m_close = true;
	}

	if (glfwGetKey(Application::_window, GLFW_KEY_1))
	{
		manager->ChangeScene(Scene::CAMERA, this);
	}

}

void MainApp::draw() 
{
	auto viewProjection = m_camera->getProjectionView();
	glm::vec3 lightColor = glm::vec3(1, 1, 0);
	glm::vec3 lightDirection = glm::vec3(0, 1, 0);
	glm::vec3 skyColor = glm::vec3(1);
	glm::vec3 groundColor = glm::vec3(0);
	glm::vec3 upVector = glm::vec3(0, 1, 0);

#pragma region Skybox
	m_shader->bind();
	m_animatedTexture->bind(GL_TEXTURE2, GL_TEXTURE_2D);

	auto textureVPUniform = m_shader->getUniform("WVP");
	auto vertexDiffMapUniform = m_shader->getUniform("diffuseMap");
	auto timeUniform = m_shader->getUniform("time");

	glUniform1i(vertexDiffMapUniform, 2); //TELL SHADER PROGRAM WHICH SHADER SLOT TO LOAD FROM
	glUniform1f(timeUniform, runningTime); //SEND THE FRAGMENT SHADER 'DELTATIME'

	glUniformMatrix4fv(textureVPUniform, 1, GL_FALSE, glm::value_ptr(viewProjection * skyboxTransform));

	m_sphere->draw(GL_TRIANGLES);
	m_shader->unbind();
#pragma endregion

}

void MainApp::OnGUI() {}