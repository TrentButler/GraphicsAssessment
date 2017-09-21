#include "LightingApp.h"
#include <glfw3.h>
#include <Mesh.h>
#include <Shader.h>
#include <FlyCamera.h>

#include <mat4x4.hpp>
#include <vec4.hpp>
#include <ext.hpp>
#include <gtc/constants.hpp>

#pragma region 4.Ability to render a plane with predefined vertex information.
Mesh* generatePlane(int width, int height)
{
	Mesh* plane = new Mesh();

	Vertex a = { glm::vec4(0, 0, 0, 1), glm::vec4(0.0f, 0.5f, 0.2f, 1.0f), glm::vec4(0), glm::vec4(0) }; //BOTTOM LEFT
	Vertex b = { glm::vec4(width, 0, 0, 1), glm::vec4(0.0f, 0.5f, 0.2f, 1.0f), glm::vec4(0), glm::vec4(0) }; //BOTTOM RIGHT
	Vertex c = { glm::vec4(width, 0, height, 1), glm::vec4(0.0f, 0.5f, 0.2f, 1.0f), glm::vec4(0), glm::vec4(0) }; //TOP RIGHT
	Vertex d = { glm::vec4(0, 0, height, 1), glm::vec4(0.0f, 0.5f, 0.2f, 1.0f), glm::vec4(0), glm::vec4(0) }; //TOP LEFT

	std::vector<Vertex> verts = { a, b, c, d };
	std::vector<unsigned int> indes = { 0, 1, 3, 3, 2, 1 };
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

			//vertex->tangent = glm::vec4(glm::sin(segment * segmentAngle + glm::half_pi<float>()), 0, glm::cos(segment * segmentAngle + glm::half_pi<float>()), 0);

			//// not a part of the AIEVertex, but this is how w generate bitangents
			//vertex->bitangent = glm::vec4(glm::cross(glm::vec3(vertex->normal), glm::vec3(vertex->tangent)), 0);

			//vertex->texcoord = glm::vec2(segment / (float)segments, ring / (float)(rings + 1));
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

/*
Evidence that includes:
Diffuse implementation
Ambient implementation
Specular implementation
Understand difference between Blinn-Phong and Phong
*/

LightingApp::LightingApp() {}
LightingApp::~LightingApp() {}

void LightingApp::startup()
{
	m_camera = new FlyCamera();
	m_camera->setLookAt(glm::vec3(-20.1f, 50.0f, -100.1f), glm::vec3(0), glm::vec3(0, 1, 0));
	m_camera->setPerspective(glm::pi<float>() / 4, (float)_width / (float)_height, 0.1f, 10000.0f);

	m_shader = new Shader();
	m_shader->load("basicShader.vert", GL_VERTEX_SHADER);
	m_shader->load("basicShader.frag", GL_FRAGMENT_SHADER);
	m_shader->attach();

#pragma region BunnyShaders
	m_diffuse = new Shader();
	m_diffuse->load("diffuse.vert", GL_VERTEX_SHADER);
	m_diffuse->load("diffuse.frag", GL_FRAGMENT_SHADER);
	m_diffuse->attach();

	m_specular = new Shader();
	m_specular->load("specular.vert", GL_VERTEX_SHADER);
	m_specular->load("specular.frag", GL_FRAGMENT_SHADER);
	m_specular->attach();

	m_phong = new Shader();
	m_phong->load("phong.vert", GL_VERTEX_SHADER);
	m_phong->load("phong.frag", GL_FRAGMENT_SHADER);
	m_phong->attach();

#pragma endregion

	m_plane = generatePlane(100, 100);

	m_loadOBJ = new Mesh();
	m_loadOBJ->loadOBJ("..//[bin]//objects//Tree", "Tree.obj");

	for (int i = 0; i < 5; i++)
	{
		Mesh* bunny = new Mesh();
		bunny->loadOBJ("..//[bin]//objects//stanford", "Bunny.obj");
		m_bunnies.push_back(bunny);
	}

	unsigned int vao = 0, vbo = 0, ibo = 0, indexcount = 0;
	m_sphere = generateSphere(100, 100, vao, vbo, ibo, indexcount);
}

void LightingApp::shutdown()
{
}

glm::mat4 loadOBJTransform = glm::mat4(1);
#pragma region BunnyTransforms
glm::mat4 sphereTransform = glm::mat4(1);
glm::mat4 bunny0Transform = glm::mat4(1);
glm::mat4 bunny1Transform = glm::mat4(1);
glm::mat4 bunny2Transform = glm::mat4(1);
glm::mat4 bunny3Transform = glm::mat4(1);
glm::mat4 bunny4Transform = glm::mat4(1);
#pragma endregion
void LightingApp::update(float deltaTime)
{
	//CAMERA STUFF	
	if (glfwGetMouseButton(Application::_window, 0) == true) //LOOK AT CENTER (0,0,0)
	{
		m_camera->setLookAt(m_camera->getWorldTransform()[3], glm::vec4(0), glm::vec3(0, 1, 0));
	}

	if (glfwGetMouseButton(Application::_window, 1) == true) //LOOK AT CENTER (0,0,0)
	{
		m_camera->setLookAt(m_camera->getWorldTransform()[3], sphereTransform[3], glm::vec3(0, 1, 0));
	}

	if (glfwGetKey(Application::_window, GLFW_KEY_W))
	{
		//MOVE FOWARD (-Z)
		//GET PREVIOUS POSITION, TRANSLATE, THEN SET IT
		auto Previous = m_camera->getWorldTransform();
		auto Translation = glm::mat4(glm::vec4(1, 0, 0, 0), glm::vec4(0, 1, 0, 0),
			glm::vec4(0, 0, 1, 0), glm::vec4(0, 0, -100 * deltaTime, 1));

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

	if (glfwGetKey(Application::_window, GLFW_KEY_0))
	{
		m_camera->setLookAt(m_camera->getWorldTransform()[3], loadOBJTransform[3], glm::vec3(0, 1, 0));
	}

	if (glfwGetKey(Application::_window, GLFW_KEY_KP_0))
	{
		m_camera->setLookAt(m_camera->getWorldTransform()[3], bunny0Transform[3], glm::vec3(0, 1, 0));
	}
	if (glfwGetKey(Application::_window, GLFW_KEY_KP_1))
	{
		m_camera->setLookAt(m_camera->getWorldTransform()[3], bunny1Transform[3], glm::vec3(0, 1, 0));
	}
	if (glfwGetKey(Application::_window, GLFW_KEY_KP_2))
	{
		m_camera->setLookAt(m_camera->getWorldTransform()[3], bunny2Transform[3], glm::vec3(0, 1, 0));
	}
	if (glfwGetKey(Application::_window, GLFW_KEY_KP_3))
	{
		m_camera->setLookAt(m_camera->getWorldTransform()[3], bunny3Transform[3], glm::vec3(0, 1, 0));
	}
	if (glfwGetKey(Application::_window, GLFW_KEY_KP_4))
	{
		m_camera->setLookAt(m_camera->getWorldTransform()[3], bunny4Transform[3], glm::vec3(0, 1, 0));
	}

	auto loadOBJTranslation = glm::mat4(
		glm::vec4(1, 0, 0, 0),
		glm::vec4(0, 1, 0, 0),
		glm::vec4(0, 0, 1, 0),
		glm::vec4(90, 0, 90, 1)
	);
	loadOBJTransform = loadOBJTranslation;

	auto sphereTranslation = glm::mat4(
		glm::vec4(1, 0, 0, 0),
		glm::vec4(0, 1, 0, 0),
		glm::vec4(0, 0, 1, 0),
		glm::vec4(50, 50, 50, 1)
	);
	sphereTransform = sphereTranslation * glm::scale(glm::vec3(10));

#pragma region BunnyTranslations
	float x = 10;
	glm::mat4 bunnyTranslation = glm::mat4(
		glm::vec4(1, 0, 0, 0),
		glm::vec4(0, 1, 0, 0),
		glm::vec4(0, 0, 1, 0),
		glm::vec4(x, 0, 50, 1)
	);	
	bunny0Transform = bunnyTranslation;
	x += 20;
	bunnyTranslation = glm::mat4(
		glm::vec4(1, 0, 0, 0),
		glm::vec4(0, 1, 0, 0),
		glm::vec4(0, 0, 1, 0),
		glm::vec4(x, 0, 50, 1)
	);
	bunny1Transform = bunnyTranslation;
	x += 20;
	bunnyTranslation = glm::mat4(
		glm::vec4(1, 0, 0, 0),
		glm::vec4(0, 1, 0, 0),
		glm::vec4(0, 0, 1, 0),
		glm::vec4(x, 0, 50, 1)
	);
	bunny2Transform = bunnyTranslation;
	x += 20;
	bunnyTranslation = glm::mat4(
		glm::vec4(1, 0, 0, 0),
		glm::vec4(0, 1, 0, 0),
		glm::vec4(0, 0, 1, 0),
		glm::vec4(x, 0, 50, 1)
	);
	bunny3Transform = bunnyTranslation;
	x += 20;
	bunnyTranslation = glm::mat4(
		glm::vec4(1, 0, 0, 0),
		glm::vec4(0, 1, 0, 0),
		glm::vec4(0, 0, 1, 0),
		glm::vec4(x, 0, 50, 1)
	);
	bunny4Transform = bunnyTranslation;
#pragma endregion

}

void LightingApp::draw()
{
	auto viewProjection = m_camera->getProjectionView();
	unsigned int defaultVPUniform = 0;
	unsigned int lightingVPUniform = 0;
	glm::vec3 lightColor = glm::vec3(1);
	glm::vec3 lightDirection = glm::vec3(0, 1, 0);

	

#pragma region Plane
	m_shader->bind();
	defaultVPUniform = m_shader->getUniform("WVP");
	glUniformMatrix4fv(defaultVPUniform, 1, GL_FALSE, glm::value_ptr(viewProjection * glm::mat4(1)));
	m_plane->draw(GL_TRIANGLES);
	m_shader->unbind();
#pragma endregion

#pragma region Sphere
	m_phong->bind();
	lightingVPUniform = m_phong->getUniform("WVP");
	auto lightingDirUniform = m_phong->getUniform("lightDirection");
	auto lightingColorUniform = m_phong->getUniform("lightColor");
	auto lightingCameraUniform = m_phong->getUniform("cameraPosition");
	auto lightingSpecularUniform = m_phong->getUniform("specularPower");
	glUniform3fv(lightingDirUniform, 1, glm::value_ptr(glm::vec3(0.0f, 1.0f, 0.0f))); // SEND THE LIGHTING SHADER THE LIGHTS DIRECTION
	glUniform3fv(lightingColorUniform, 1, glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f))); // SEND THE LIGHTING SHADER THE LIGHTS COLOR
	glUniform3fv(lightingCameraUniform, 1, glm::value_ptr(m_camera->getView()[3])); // SEND THE LIGHTING SHADER THE CAMERA'S POSITION
	glUniform1f(lightingSpecularUniform, 128.0f); // SEND THE LIGHTING SHADER A VALUE FOR THE SPECULAR POWER
	glUniformMatrix4fv(lightingVPUniform, 1, GL_FALSE, glm::value_ptr(viewProjection * sphereTransform));
	m_sphere->draw(GL_TRIANGLES);
	m_phong->unbind();
#pragma endregion

#pragma region LoadedObject
	m_phong->bind();
	lightingVPUniform = m_phong->getUniform("WVP");
	auto loadObjectLightDirectionUniform = m_phong->getUniform("lightDirection");
	auto loadObjectColorUniform = m_phong->getUniform("lightColor");
	auto loadObjectCameraUniform = m_phong->getUniform("cameraPosition");
	auto loadObjectSpecularPowerUniform = m_phong->getUniform("specularPower");
	glUniform3fv(loadObjectLightDirectionUniform, 1, glm::value_ptr(glm::vec3(0.0f, 1.0f, 0.0f))); // SEND THE PHONG SHADER THE LIGHTS DIRECTION
	glUniform3fv(loadObjectColorUniform, 1, glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f))); // SEND THE PHONG SHADER THE LIGHTS COLOR
	glUniform3fv(loadObjectCameraUniform, 1, glm::value_ptr(m_camera->getView()[3])); // SEND THE PHONG SHADER THE CAMERA'S POSITION
	glUniform1f(loadObjectSpecularPowerUniform, 128.0f); // SEND THE PHONG SHADER A VALUE FOR THE SPECULAR POWER
	glUniformMatrix4fv(lightingVPUniform, 1, GL_FALSE, glm::value_ptr(viewProjection * loadOBJTransform));
	m_loadOBJ->draw(GL_TRIANGLES);
	m_phong->unbind();
#pragma endregion

#pragma region Bunny0 //AMBIENT LIGHTING
	//unsigned int ambientVPUniform = m_ambient->getUniform("WVP");
	m_shader->bind();
	defaultVPUniform = m_shader->getUniform("WVP"); //USE 'lightingVPUniform'
	glUniformMatrix4fv(defaultVPUniform, 1, GL_FALSE, glm::value_ptr(viewProjection * bunny0Transform));
	m_bunnies[0]->draw(GL_TRIANGLES);
	m_shader->unbind();
#pragma endregion

#pragma region Bunny1 //DIFFUSE LIGHTING
	m_diffuse->bind();
	lightingVPUniform = m_diffuse->getUniform("WVP");
	auto diffuseLightDirectionUniform = m_diffuse->getUniform("lightDirection"); //GET HANDLE FOR THE DIFFUSE SHADER UNIFORM 'lightDirection'
	auto diffuseLightColorUniform = m_diffuse->getUniform("lightColor"); //GET HANDLE FOR THE DIFFUSE SHADER UNIFORM 'lightColor'
	
	glUniform3fv(diffuseLightDirectionUniform, 1, glm::value_ptr(lightDirection)); //SEND THE DIFFUSE SHADER THE LIGHT'S DIRECTION
	glUniform3fv(diffuseLightColorUniform, 1, glm::value_ptr(lightColor)); //SEND THE DIFFUSE SHADER THE LIGHT'S COLOR
	glUniformMatrix4fv(lightingVPUniform, 1, GL_FALSE, glm::value_ptr(viewProjection * bunny1Transform));
	m_bunnies[1]->draw(GL_TRIANGLES);
	m_diffuse->unbind();
#pragma endregion

#pragma region Bunny2 //SPECULAR LIGHTING
	m_specular->bind();
	lightingVPUniform = m_specular->getUniform("WVP");
	auto specularLightDirectionUniform = m_specular->getUniform("lightDirection"); //GET HANDLE FOR THE SPECULAR SHADER UNIFORM 'lightDirection'
	auto specularLightColorUniform = m_specular->getUniform("lightColor"); //GET HANDLE FOR THE SPECULAR SHADER UNIFORM 'lightColor'
	auto specularCameraPosUniform = m_specular->getUniform("cameraPosition"); //GET HANDLE FOR THE SPECULAR SHADER UNIFORM 'cameraPosition'
	auto specularPowerUniform = m_specular->getUniform("specularPower"); //GET HANDLE FOR THE SPECULAR SHADER UNIFORM 'specularPower'
	
	glUniform3fv(specularLightDirectionUniform, 1, glm::value_ptr(lightDirection)); //SEND THE SPECULAR SHADER THE LIGHT'S DIRECTION
	glUniform3fv(specularLightColorUniform, 1, glm::value_ptr(lightColor)); //SEND THE SPECULAR SHADER THE LIGHT'S COLOR
	glUniform3fv(specularCameraPosUniform, 1, glm::value_ptr(m_camera->getView()[3])); //SEND THE SPECULAR SHADER THE CAMERA'S POSITION
	glUniform1f(specularPowerUniform, 128.0f); //SEND THE SPECULAR SHADER A VALUE FOR 'specularPower'
	glUniformMatrix4fv(lightingVPUniform, 1, GL_FALSE, glm::value_ptr(viewProjection * bunny2Transform));
	m_bunnies[2]->draw(GL_TRIANGLES);
	m_specular->unbind();
#pragma endregion

#pragma region Bunny3 //PHONG LIGHTING	
	m_phong->bind();
	lightingVPUniform = m_phong->getUniform("WVP");
	
	auto phongLightDirectionUniform = m_phong->getUniform("lightDirection");
	auto phongLightColorUniform = m_phong->getUniform("lightColor");
	auto phongCameraPosUniform = m_phong->getUniform("cameraPosition");
	auto phongSpecularPowerUniform = m_phong->getUniform("specularPower");
	glUniform3fv(phongLightDirectionUniform, 1, glm::value_ptr(glm::vec3(0.0f, 1.0f, 0.0f))); // SEND THE PHONG SHADER THE LIGHTS DIRECTION
	glUniform3fv(phongLightColorUniform, 1, glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f))); // SEND THE PHONG SHADER THE LIGHTS COLOR
	glUniform3fv(phongCameraPosUniform, 1, glm::value_ptr(m_camera->getView()[3])); // SEND THE PHONG SHADER THE CAMERA'S POSITION
	glUniform1f(phongSpecularPowerUniform, 128.0f); // SEND THE PHONG SHADER A VALUE FOR THE SPECULAR POWER
	glUniformMatrix4fv(lightingVPUniform, 1, GL_FALSE, glm::value_ptr(viewProjection * bunny3Transform));
	m_bunnies[3]->draw(GL_TRIANGLES);
	m_phong->unbind();
#pragma endregion

#pragma region Bunny4 //BLINN-PHONG LIGHTING
	//unsigned int blinnphongVPUniform = m_ambient->getUniform("WVP");
	m_shader->bind();
	defaultVPUniform = m_shader->getUniform("WVP");
	glUniformMatrix4fv(defaultVPUniform, 1, GL_FALSE, glm::value_ptr(viewProjection * bunny4Transform));
	m_bunnies[4]->draw(GL_TRIANGLES);
	m_shader->unbind();
#pragma endregion
}