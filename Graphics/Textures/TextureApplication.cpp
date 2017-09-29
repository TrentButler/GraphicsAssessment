#include "TextureApplication.h"
#include <FlyCamera.h>
#include <Mesh.h>
#include <Shader.h>
#include <Texture.h>
#include <glfw3.h>
#include <gtc/constants.hpp>
#include <ext.hpp>
#include <imgui.h>
#include <imgui_impl_glfw_gl3.h>
#include <iostream>

#pragma region 4.Ability to render a plane with predefined vertex information.
Mesh* generatePlane(int width, int height)
{
	Mesh* plane = new Mesh();

	Vertex a = { glm::vec4(0, 0, 0, 1), glm::vec4(0.0f, 0.5f, 0.2f, 1.0f), glm::vec4(0, 1, 0, 1), glm::vec3(0, 0, 0), glm::vec4(1,0,0,0) }; //BOTTOM LEFT
	Vertex b = { glm::vec4(width, 0, 0, 1), glm::vec4(0.0f, 0.5f, 0.2f, 1.0f), glm::vec4(0, 1, 0, 1), glm::vec3(1, 0, 0), glm::vec4(1,0,0,0) }; //BOTTOM RIGHT
	Vertex c = { glm::vec4(width, 0, height, 1), glm::vec4(0.0f, 0.5f, 0.2f, 1.0f), glm::vec4(0, 1, 0, 1), glm::vec3(1, 1, 0), glm::vec4(1,0,0,0) }; //TOP RIGHT
	Vertex d = { glm::vec4(0, 0, height, 1), glm::vec4(0.0f, 0.5f, 0.2f, 1.0f), glm::vec4(0, 1, 0, 1), glm::vec3(0, 1, 0), glm::vec4(1,0,0,0) }; //TOP LEFT

	std::vector<Vertex> verts = { a, b, c, d };
	std::vector<unsigned int> indes = { 0, 1, 3, 3, 2, 1 }; //look here
	plane->initialize(verts, indes);

	return plane;
}
#pragma endregion

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


TextureApplication::TextureApplication() {};
TextureApplication::~TextureApplication() {};
float runningTime = 0;

void TextureApplication::startup()
{
	m_camera = new FlyCamera();
	//m_camera->setLookAt(glm::vec3(-20.1f, 50.0f, -100.1f), glm::vec3(0), glm::vec3(0, 1, 0));
	m_camera->setLookAt(glm::vec3(-20.1f, 50.0f, -100.1f), glm::vec3(0), glm::vec3(0, 1, 0));
	m_camera->setPerspective(glm::pi<float>() / 4, (float)_width / (float)_height, 0.1f, 10000.0f);
	
	m_plane = generatePlane(100, 100);
	unsigned int vao, vbo, ibo, indexcount;	
	m_sphere = generateSphere(100, 100, vao, vbo, ibo, indexcount);

	m_multTexShader = new Shader();
	m_multTexShader->load("multiTextureShader.vert", GL_VERTEX_SHADER);
	m_multTexShader->load("multiTextureShader.frag", GL_FRAGMENT_SHADER);
	m_multTexShader->attach();

	/*m_texture = new Texture();
	m_texture->load2D("..//[bin]//textures", "skyboxTexture.jpg");

	m_planeTexture = new Texture();
	m_planeTexture->load2D("..//[bin]//textures", "debugTexture.jpg");*/

	m_diffuseMap = new Texture();
	m_diffuseMap->load2D("..//[bin]//textures//diffuse", "starfieldDiffuseMap.jpg");
	
	m_normalMap = new Texture();
	m_normalMap->load2D("..//[bin]//textures//normal", "151_norm.JPG");
}

void TextureApplication::shutdown()
{
}

glm::mat4 sphereTransform = glm::mat4(1);
glm::mat4 planeTransform = glm::mat4(1);
glm::vec2 deltaUV = glm::vec2(0);
void TextureApplication::update(float deltaTime)
{
	runningTime += deltaTime;

	sphereTransform = glm::scale(glm::vec3(1000));	
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
		m_multTexShader->load("multiTextureShader.vert", GL_VERTEX_SHADER);
		m_multTexShader->load("multiTextureShader.frag", GL_FRAGMENT_SHADER);
		m_multTexShader->attach();
	}

	if (glfwGetKey(Application::_window, GLFW_KEY_0))
	{
		m_camera->setLookAt(m_camera->getWorldTransform()[3], planeTransform[3], glm::vec3(0, 1, 0));
	}

	if (glfwGetKey(Application::_window, GLFW_KEY_KP_0))
	{
		m_camera->setLookAt(m_camera->getWorldTransform()[3], sphereTransform[3], glm::vec3(0, 1, 0));
	}

}

void TextureApplication::draw()
{
	auto viewProjection = m_camera->getProjectionView();
	glm::vec3 lightColor = glm::vec3(1, 1, 0);
	glm::vec3 lightDirection = glm::vec3(0, 1, 0);
	glm::vec3 skyColor = glm::vec3(1);
	glm::vec3 groundColor = glm::vec3(0);
	glm::vec3 upVector = glm::vec3(0, 1, 0);

	/*m_shader->bind();
	m_texture->bind(GL_TEXTURE0, GL_TEXTURE_2D);	
	glUniform1i(vertexTextureUniform, 0);
	glUniformMatrix4fv(textureVPUniform, 1, GL_FALSE, glm::value_ptr(viewProjection * sphereTransform));	
	m_sphere->draw(GL_TRIANGLES);
	m_shader->unbind();*/

	m_multTexShader->bind();
	m_diffuseMap->bind(GL_TEXTURE0, GL_TEXTURE_2D);
	m_normalMap->bind(GL_TEXTURE1, GL_TEXTURE_2D);

	auto textureVPUniform = m_multTexShader->getUniform("WVP");

	auto lightDirectionUniform = m_multTexShader->getUniform("lightDirection");

	auto vertexDiffMapUniform = m_multTexShader->getUniform("diffuseMap");
	auto vertexNormMapUniform = m_multTexShader->getUniform("normalMap");

	auto timeUniform = m_multTexShader->getUniform("time");

	glUniform3fv(lightDirectionUniform, 1, glm::value_ptr(lightDirection)); // SEND THE PHONG SHADER THE LIGHTS DIRECTION	
	glUniform1i(vertexDiffMapUniform, 0); //TELL SHADER PROGRAM WHICH SHADER SLOT TO LOAD FROM
	glUniform1i(vertexNormMapUniform, 1); //TELL SHADER PROGRAM WHICH SHADER SLOT TO LOAD FROM
	glUniform1f(timeUniform, runningTime); //SEND THE FRAGMENT SHADER 'DELTATIME'

	glUniformMatrix4fv(textureVPUniform, 1, GL_FALSE, glm::value_ptr(viewProjection * sphereTransform));
	m_sphere->draw(GL_TRIANGLES);
	m_multTexShader->unbind();
}

void TextureApplication::OnGUI()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("FILE"))
		{
			if (ImGui::MenuItem("NEW GEOMETRY")) {}
			if (ImGui::MenuItem("NEW VERTEX SHADER")) {}
			if (ImGui::MenuItem("NEW FRAGMENT SHADER")) {}
			ImGui::EndMenu(); //END THE MENU
		}

		if (ImGui::BeginMenu("EDIT"))
		{

			ImGui::EndMenu(); //END THE MENU
		}
		ImGui::EndMainMenuBar(); //END THE MAIN MENU BAR
	}
}
