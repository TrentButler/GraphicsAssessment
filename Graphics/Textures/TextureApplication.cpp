#include "TextureApplication.h"
#include <FlyCamera.h>
#include <Mesh.h>
#include <Shader.h>
#include <Texture.h>
#include <glfw3.h>
#include <gtc/constants.hpp>
#include <ext.hpp>

#pragma region 4.Ability to render a plane with predefined vertex information.
Mesh* generatePlane(int width, int height)
{
	Mesh* plane = new Mesh();

	Vertex a = { glm::vec4(0, 0, 0, 1), glm::vec4(0.0f, 0.5f, 0.2f, 1.0f), glm::vec4(0), glm::vec3(0, 0, 0) }; //BOTTOM LEFT
	Vertex b = { glm::vec4(width, 0, 0, 1), glm::vec4(0.0f, 0.5f, 0.2f, 1.0f), glm::vec4(0), glm::vec3(1, 0, 0) }; //BOTTOM RIGHT
	Vertex c = { glm::vec4(width, 0, height, 1), glm::vec4(0.0f, 0.5f, 0.2f, 1.0f), glm::vec4(0), glm::vec3(1, 1, 0) }; //TOP RIGHT
	Vertex d = { glm::vec4(0, 0, height, 1), glm::vec4(0.0f, 0.5f, 0.2f, 1.0f), glm::vec4(0), glm::vec3(0, 1, 0) }; //TOP LEFT

	std::vector<Vertex> verts = { a, b, c, d };
	std::vector<unsigned int> indes = { 0, 1, 3, 3, 2, 1 }; //look here
	plane->initialize(verts, indes);

	return plane;
}
#pragma endregion


TextureApplication::TextureApplication() {};
TextureApplication::~TextureApplication() {};


void TextureApplication::startup()
{
	m_camera = new FlyCamera();
	m_camera->setLookAt(glm::vec3(-20.1f, 50.0f, -100.1f), glm::vec3(0), glm::vec3(0, 1, 0));
	m_camera->setPerspective(glm::pi<float>() / 4, (float)_width / (float)_height, 0.1f, 10000.0f);
	
	m_plane = generatePlane(100, 100);

	m_shader = new Shader();
	m_shader->load("textureShader.vert", GL_VERTEX_SHADER);
	m_shader->load("textureShader.frag", GL_FRAGMENT_SHADER);
	m_shader->attach();

	m_texture = new Texture();
	m_texture->load("..//[bin]//textures", "leatherTexture.jpg");
}

void TextureApplication::shutdown()
{
}

glm::mat4 planeTransform = glm::mat4(1);
void TextureApplication::update(float deltaTime)
{
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

	if (glfwGetKey(Application::_window, GLFW_KEY_0))
	{
		m_camera->setLookAt(m_camera->getWorldTransform()[3], planeTransform[3], glm::vec3(0, 1, 0));
	}
}

void TextureApplication::draw()
{
	m_shader->bind();
	m_texture->bind();

	auto viewProjection = m_camera->getProjectionView();
	auto textureVPUniform = m_shader->getUniform("WVP");
	auto vertexTextureUniform = m_shader->getUniform("vertexTexture");

	glUniform1i(vertexTextureUniform, 0);
	glUniformMatrix4fv(textureVPUniform, 1, GL_FALSE, glm::value_ptr(viewProjection * planeTransform));
	
	m_plane->draw(GL_TRIANGLES);
	m_shader->unbind();
}
