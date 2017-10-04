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
#include <vector>

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

#pragma region GeneratePlane
Mesh* generateGrid(unsigned int rows, unsigned int cols)
{
	auto aoVertices = new Vertex[rows * cols];
	for (unsigned int r = 0; r < rows; ++r)
	{
		for (unsigned int c = 0; c < cols; ++c)
		{
			Vertex verts = {
				glm::vec4(float(c), 0, float(r), 1), //POSITION
				glm::vec4(0), //COLOR
				glm::vec4(0, 1, 0, 0), //NORMAL
				glm::vec3(float(c) / float(cols - 1), float(r) / float(rows - 1), 0), //TEXTURE COORDINATE
				glm::vec4(0,1,0,0) //TANGENT
			};
			aoVertices[r * cols + c] = verts;
		}
	}

	std::vector<Vertex> verts = std::vector<Vertex>();
	std::vector<unsigned int> indices = std::vector<unsigned int>();

	//Defining index count based off quad count (2 triangles per quad)
	unsigned int* auiIndices = new unsigned int[(rows - 1) * (cols - 1) * 6];
	unsigned int index = 0;
	for (unsigned int r = 0; r < (rows - 1); ++r)
	{
		for (unsigned int c = 0; c < (cols - 1); ++c)
		{
			//Triangle 1
			auiIndices[index++] = r * cols + c;
			auiIndices[index++] = (r + 1) * cols + c;
			auiIndices[index++] = (r + 1) * cols + (c + 1);
			//Triangle 2
			auiIndices[index++] = r * cols + c;
			auiIndices[index++] = (r + 1) * cols + (c + 1);
			auiIndices[index++] = r * cols + (c + 1);
		}
	}

	for (unsigned int i = 0; i < (rows * cols); i++)
	{
		verts.push_back(aoVertices[i]);
	}
		
	for (unsigned int i = 0; i < index; i++)
	{
		indices.push_back(auiIndices[i]);
	}
	
	Mesh* plane = new Mesh();
	plane->initialize(verts, indices);

	delete[] aoVertices;
	delete[] auiIndices;
	return plane;
}
#pragma endregion

#pragma region PerlinNoise
float trentNoise()
{
	//MAKE NOISE HERE
	return 0.0f;
}

std::vector<float> generateNoiseTexture(unsigned int width, unsigned int height)
{
	std::vector<float> noiseTexture;

	int dims = width;
	float scale = (1.0f / dims) * 3;
	int octaves = 6;

	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			noiseTexture.push_back(glm::perlin(glm::vec2(i, j) * scale) * 0.5f + 0.5f);
		}
	}
	
	for (int x = 0; x < width; x++)
	{
		for (int y = 0; y < height; y++)
		{
			float amplitude = 1.0f;
			float persistence = 0.3f;
			noiseTexture[y * dims + x] = 0.0f;
			for (int o = 0; o < octaves; o++)
			{
				float freq = powf(2, float(o));
				float perlinSample = glm::perlin(glm::vec2(float(x), float(y)) * scale * freq) * 0.5f + 0.5f;
				noiseTexture[y * dims + x] += perlinSample * amplitude;
				amplitude *= persistence;
			}
		}
	}
	
	return noiseTexture;
}
#pragma endregion

TextureApplication::TextureApplication() {};
TextureApplication::~TextureApplication() {};

void TextureApplication::startup()
{
	m_camera = new FlyCamera();
	//m_camera->setLookAt(glm::vec3(-20.1f, 50.0f, -100.1f), glm::vec3(0), glm::vec3(0, 1, 0));
	m_camera->setLookAt(glm::vec3(-20.1f, 50.0f, -150.1f), glm::vec3(150, 0, 0), glm::vec3(0, 1, 0));
	m_camera->setPerspective(glm::pi<float>() / 4, (float)_width / (float)_height, 0.1f, 10000.0f);
	
#pragma region Objects
	/*unsigned int Svao, Svbo, Sibo, Sindexcount;
	m_perlinMesh = generateSphere(100, 100, Svao, Svbo, Sibo, Sindexcount);*/

	/*m_perlinMesh = new Mesh();
	m_perlinMesh->loadOBJ("..//[bin]//objects//basic", "64x64plane.obj");*/
	m_perlinMesh = generateGrid(100, 100);

	/*m_perlinMesh = new Mesh();
	m_perlinMesh->loadOBJ("..//[bin]//objects/Heart", "heart.obj");*/

	//m_plane = generatePlane(100, 100);
	//m_plane = new Mesh();
	//m_plane->loadOBJ("..//[bin]//objects//basic", "64x64plane.obj");
	m_plane = generateGrid(100, 100);

	unsigned int vao, vbo, ibo, indexcount;	
	m_sphere = generateSphere(100, 100, vao, vbo, ibo, indexcount);
#pragma endregion

#pragma region Shaders
	m_multTexShader = new Shader();
	m_multTexShader->load("multiTextureShader.vert", GL_VERTEX_SHADER);
	m_multTexShader->load("multiTextureShader.frag", GL_FRAGMENT_SHADER);
	m_multTexShader->attach();

	m_shader = new Shader();
	m_shader->load("textureShader.vert", GL_VERTEX_SHADER);
	m_shader->load("textureShader.frag", GL_FRAGMENT_SHADER);
	m_shader->attach();

	m_perlinShader = new Shader();
	m_perlinShader->load("perlinShader.vert", GL_VERTEX_SHADER);
	m_perlinShader->load("perlinShader.frag", GL_FRAGMENT_SHADER);
	m_perlinShader->attach();
#pragma endregion

#pragma region Textures
	m_diffuseMap = new Texture();
	m_diffuseMap->load2D("..//[bin]//textures//diffuse", "151.JPG");
	
	m_normalMap = new Texture();
	m_normalMap->load2D("..//[bin]//textures//normal", "151_norm.JPG");

	m_animatedTexture = new Texture();
	m_animatedTexture->load2D("..//[bin]//textures//diffuse", "starfieldDiffuseMap.jpg");

	m_perlinTexture = new Texture();
	m_perlinTexture->generate2D(64, 64, generateNoiseTexture(64, 64));
#pragma endregion
}

void TextureApplication::shutdown()
{
	delete m_camera;
	delete m_plane;
	delete m_sphere;
	delete m_perlinMesh;

	delete m_shader;
	delete m_multTexShader;
	delete m_perlinShader;

	delete m_diffuseMap;
	delete m_normalMap;
	delete m_animatedTexture;
	delete m_perlinTexture;	
	return;
}

glm::mat4 sphereTransform = glm::mat4(1);
glm::mat4 planeTransform = glm::mat4(1);
glm::mat4 perlinPlaneTransform = glm::mat4(1);
glm::vec2 deltaUV = glm::vec2(0);
float runningTime = 0;
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
		
		m_shader->load("textureShader.vert", GL_VERTEX_SHADER);
		m_shader->load("textureShader.frag", GL_FRAGMENT_SHADER);
		m_shader->attach();
		
		m_perlinShader->load("perlinShader.vert", GL_VERTEX_SHADER);
		m_perlinShader->load("perlinShader.frag", GL_FRAGMENT_SHADER);
		m_perlinShader->attach();
	}

	if (glfwGetKey(Application::_window, GLFW_KEY_0))
	{
		m_camera->setLookAt(m_camera->getWorldTransform()[3], planeTransform[3], glm::vec3(0, 1, 0));
	}

	if (glfwGetKey(Application::_window, GLFW_KEY_KP_0))
	{
		m_camera->setLookAt(m_camera->getWorldTransform()[3], sphereTransform[3], glm::vec3(0, 1, 0));
	}

	if (glfwGetKey(Application::_window, GLFW_KEY_KP_1))
	{
		m_camera->setLookAt(m_camera->getWorldTransform()[3], perlinPlaneTransform[3], glm::vec3(0, 1, 0));
	}

	if (glfwGetKey(Application::_window, GLFW_KEY_ESCAPE))
	{
		m_close = true;
	}

	auto perlinTranslation = glm::mat4(
		glm::vec4(1, 0, 0, 0),
		glm::vec4(0, 1, 0, 0),
		glm::vec4(0, 0, 1, 0),
		glm::vec4(150, 0, 0, 1)
	);

	perlinPlaneTransform = perlinTranslation * glm::scale(glm::vec3(1)) * planeTransform;
}

void TextureApplication::draw()
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

	glUniformMatrix4fv(textureVPUniform, 1, GL_FALSE, glm::value_ptr(viewProjection * sphereTransform));

	m_sphere->draw(GL_TRIANGLES);
	m_shader->unbind();
#pragma endregion

#pragma region BumpMappedPlane
	m_multTexShader->bind();

	m_diffuseMap->bind(GL_TEXTURE0, GL_TEXTURE_2D);
	m_normalMap->bind(GL_TEXTURE1, GL_TEXTURE_2D);	
	m_perlinTexture->bind(GL_TEXTURE2, GL_TEXTURE_2D);

	auto bumpmapVPUniform = m_multTexShader->getUniform("WVP");
	auto bumpmaplightDirectionUniform = m_multTexShader->getUniform("lightDirection");
	auto bumpmapDiffMapUniform = m_multTexShader->getUniform("diffuseMap");
	auto bumpmapNormMapUniform = m_multTexShader->getUniform("normalMap");
	//auto bumpmapPerlinUniform = m_multTexShader->getUniform("perlinMap");
	
	glUniform3fv(bumpmaplightDirectionUniform, 1, glm::value_ptr(lightDirection)); // SEND THE PHONG SHADER THE LIGHTS DIRECTION	
	glUniform1i(bumpmapDiffMapUniform, 0); //TELL SHADER PROGRAM WHICH SHADER SLOT TO LOAD FROM
	glUniform1i(bumpmapNormMapUniform, 1); //TELL SHADER PROGRAM WHICH SHADER SLOT TO LOAD FROM
	
	//glUniform1i(bumpmapPerlinUniform, 3);

	glUniformMatrix4fv(bumpmapVPUniform, 1, GL_FALSE, glm::value_ptr(viewProjection * planeTransform));
	
	m_plane->draw(GL_TRIANGLES);
	m_multTexShader->unbind();
#pragma endregion

#pragma region PerlinMesh
	m_perlinShader->bind();
	m_perlinTexture->bind(GL_TEXTURE3, GL_TEXTURE_2D);

	auto perlinWVPUniform = m_perlinShader->getUniform("WVP");
	auto perlinDiffuseMapUniform = m_perlinShader->getUniform("diffuseMap");

	glUniform1i(perlinDiffuseMapUniform, 3);
	glUniformMatrix4fv(perlinWVPUniform, 1, GL_FALSE, glm::value_ptr(viewProjection * perlinPlaneTransform));
	
	m_perlinMesh->draw(GL_TRIANGLES);

	m_perlinShader->unbind();
#pragma endregion
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