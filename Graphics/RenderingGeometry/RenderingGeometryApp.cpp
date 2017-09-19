#include "RenderingGeometryApp.h"
#include "Shader.h"
#include "Mesh.h"
#include "Texture.h"
#include <glfw3.h>
#include <FlyCamera.h>
#include <vector>
#include <vec4.hpp>
#include <ext.hpp>
#include <gtc/constants.hpp>
#define PI 3.14159265359


#pragma region 1.Function that generates a half circle given a number of points and radius.
std::vector<Vertex> genHalfCircle(float radius, int numPoints)
{
	std::vector<Vertex> verts;
	auto slice = PI / numPoints - 1;
	for (int i = 0; i < numPoints; i++)
	{
		auto angle = slice * i;
		glm::vec4 point = glm::vec4(glm::cos(angle), glm::sin(angle), 0, 1);
		Vertex vert = { point, glm::vec4(1), glm::vec4(0), glm::vec3(0) };
		verts.push_back(vert);
	}

	return verts;
}
#pragma endregion

#pragma region 2.Function that generates a sphere given a half circle, and number of meridians.
std::vector<Vertex> generateSphereVerts(std::vector<Vertex> halfCircle, int numMeridians)
{
	std::vector<Vertex> halfCirc = halfCircle;
	std::vector<Vertex> verts;

	int merdians = numMeridians;
	if (numMeridians < 3)
	{
		merdians = 3;
	}

	for (auto v : halfCirc)
	{
		verts.push_back(v); //POPULATE WITH THE ORIGINAL HALF CIRCLE
	}

	//ROTATE THE POINTS BY AN 'ANGLE' (numMeridians) TIMES ON THE XAXIS AND YAXIS
	for (int i = 0; i < merdians + 1; i++)
	{
		auto slice = 360 / merdians;
		auto angle = slice * i;

		/*for (int yRotCount = 0; yRotCount < halfCirc.size(); yRotCount++)
		{
			auto YRot = glm::mat4(
				glm::vec4(cosf(angle), 0, -sinf(angle), 0),
				glm::vec4(0, 1, 0, 0),
				glm::vec4(sinf(angle), 0, cosf(angle), 0),
				glm::vec4(0, 0, 0, 1)
				);

			auto point = halfCirc[yRotCount].position;
			auto rotatedPoint = point * YRot;

			Vertex vert = { rotatedPoint, glm::vec4(1), glm::vec4(0), glm::vec3(0) };
			verts.push_back(vert);
		}*/

		for (int xrotCount = 0; xrotCount < halfCirc.size(); xrotCount++)
		{
			//ROTATE THE HALF CIRCLE BY ANGLE
			auto XRot = glm::mat4(
				glm::vec4(1, 0, 0, 0),
				glm::vec4(0, glm::cos(angle), glm::sin(angle), 0),
				glm::vec4(0, -glm::sin(angle), glm::cos(angle), 0),
				glm::vec4(0, 0, 0, 1)
				);

			auto point = halfCirc[xrotCount].position;
			auto rotatedPoint = point * XRot;

			Vertex vert = { rotatedPoint, glm::vec4(1), glm::vec4(0), glm::vec3(0) };
			verts.push_back(vert);
		}
	}

	return verts;
}
#pragma endregion

#pragma region 3.Function that generates indices for geometry to be rendered using triangle strips.

#pragma endregion

#pragma region 4.Ability to render a plane with predefined vertex information.
Mesh* generatePlane(int width, int height)
{
	Mesh* plane = new Mesh();

	Vertex a = { glm::vec4(0, 0, 0, 1), glm::vec4(0.0f, 0.0f, 1.0f, 0.2f), glm::vec4(0), glm::vec4(0) }; //BOTTOM LEFT
	Vertex b = { glm::vec4(width, 0, 0, 1), glm::vec4(0.0f, 0.0f, 1.0f, 0.2f), glm::vec4(0), glm::vec4(0) }; //BOTTOM RIGHT
	Vertex c = { glm::vec4(width, 0, height, 1), glm::vec4(0.0f, 0.0f, 1.0f, 0.2f), glm::vec4(0), glm::vec4(0) }; //TOP RIGHT
	Vertex d = { glm::vec4(0, 0, height, 1), glm::vec4(0.0f, 0.0f, 1.0f, 0.2f), glm::vec4(0), glm::vec4(0) }; //TOP LEFT	

	std::vector<Vertex> verts = { a, b, c, d };
	std::vector<unsigned int> indes = { 0, 1, 3, 3, 2, 1 };
	plane->initialize(verts, indes);

	return plane;
}
#pragma endregion

#pragma region 5.Ability to render a cube with predefined vertex information.

Mesh* generateCube(float scale)
{
	Mesh* Cube = new Mesh();

	Vertex a = { glm::vec4(0.0f,0.0f,0.0f,1.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), glm::vec4(0), glm::vec3(0) }; // BOTTOM LEFT
	Vertex b = { glm::vec4(scale,0.0f,0.0f,1.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), glm::vec4(0), glm::vec3(0) }; // BOTTOM RIGHT
	Vertex c = { glm::vec4(scale,scale,0.0f,1.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), glm::vec4(0), glm::vec3(0) }; // TOP RIGHT
	Vertex d = { glm::vec4(0.0f,scale,0.0f,1.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), glm::vec4(0), glm::vec3(0) }; // TOP LEFT
	Vertex e = { glm::vec4(0.0f,0.0f, scale,1.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), glm::vec4(0), glm::vec3(0) }; // BACK BOTTOM LEFT
	Vertex f = { glm::vec4(scale, 0.0f, scale,1.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), glm::vec4(0), glm::vec3(0) }; // BACK BACK BOTTOM RIGHT
	Vertex g = { glm::vec4(scale, scale, scale,1.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), glm::vec4(0), glm::vec3(0) }; // BACK TOP RIGHT
	Vertex h = { glm::vec4(0.0f, scale, scale,1.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), glm::vec4(0), glm::vec3(0) }; // BACK TOP LEFT

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
Mesh* generateSphere(float scale, int meridians)
{
	Mesh* sphere = new Mesh();
	int halfCircPointCount = 10;
	auto halfCirc = genHalfCircle(1, halfCircPointCount);
	auto sphereVerts = generateSphereVerts(halfCirc, meridians);

	std::vector<unsigned int> sphereIndices;

	for (int i = 0; i < sphereVerts.size(); i++)
	{
		sphereIndices.push_back(i);
	}

	sphere->initialize(sphereVerts, sphereIndices);

	return sphere;
}
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
	m_camera->setLookAt(glm::vec3(-20.1f, 50.0f, -100.1f), glm::vec3(0), glm::vec3(0, 1, 0));
	m_camera->setPerspective(glm::pi<float>() / 4, (float)_width / (float)_height, 0.1f, 10000.0f);

	m_defaultShader = new Shader();
	m_defaultShader->load("basicShader.vert", GL_VERTEX_SHADER);
	m_defaultShader->load("basicShader.frag", GL_FRAGMENT_SHADER);
	m_defaultShader->attach();

	m_textureShader = new Shader();
	m_textureShader->load("textureShader.vert", GL_VERTEX_SHADER);
	m_textureShader->load("textureShader.frag", GL_FRAGMENT_SHADER);
	m_textureShader->attach();

	m_texture = new Texture();
	m_texture->load("..//[bin]//textures", "hdFurTexture.jpg");

	m_plane = generatePlane(100, 100);
	m_cube = generateCube(10);
	m_sphere = generateSphere(10, 360);

	m_loadOBJ = new Mesh();
	m_loadOBJ->loadOBJ("..//[bin]//objects//Tree", "Tree.obj");
}

void RenderingGeometryApp::shutdown() {}

static double prevMouseX = 0; //USED TO CALCULATE THE MOUSE DELTA X
static double prevMouseY = 0; //USED TO CALCULATE THE MOUSE DELTA Y
static double yaw = 0; //VARIABLE USED TO STORE THE 'YAW' AMOUNT
static double pitch = 0; //VARIABLE USED TO STORE THE 'PITCH' AMOUNT
static glm::vec2 deltaMouse = glm::vec2(0); //VECTOR2 TO STORE THE CHANGE IN MOUSE POSITION
glm::mat4 planeTransform = glm::mat4(1);
glm::mat4 cubeTransform = glm::mat4(1);
glm::mat4 sphereTransform = glm::mat4(1);
glm::mat4 loadObjTransform = glm::mat4(1);
void RenderingGeometryApp::update(float deltaTime)
{
	//CAMERA STUFF
	bool pressed = false;
	if (glfwGetMouseButton(Application::_window, 0) == true) //MOUSE CLICKED
	{
		if (pressed == false)
		{
			pressed = true;
			glfwGetCursorPos(Application::_window, &prevMouseX, &prevMouseY);
		}

		double newMouseX = 0;
		double newMouseY = 0;
		glfwGetCursorPos(Application::_window, &newMouseX, &newMouseY);

		deltaMouse = glm::vec2((newMouseX - prevMouseX), -(newMouseY - prevMouseY)); //CALCULATE THE CHANGE IN MOUSE POSITION
		prevMouseX = newMouseX; //STORE THE NEW VALUE OF THE MOUSE X
		prevMouseY = newMouseY; //STORE THE NEW VALUE OF THE MOUSE Y

		yaw += deltaMouse.x * 0.04; //INCREMENT THE 'YAW' VALUE BASED ON DELTA MOUSE X
		pitch += deltaMouse.y * 0.04; //INCREMENT THE 'PITCH' VALUE BASED ON DELTA MOUSE Y

		if (pitch > 89) //CLAMP PITCH VALUE BETWEEN -90 AND 90
		{
			pitch = 89;
		}
		if (pitch < -89) //CLAMP PITCH VALUE BETWEEN -90 AND 90
		{
			pitch = -89;
		}

		auto Front = glm::vec3( //GENERATE AN FRONT DIRECTION FOR THE CAMERA USING THE YAW AND PITCH VALUES
			glm::cos(glm::radians(yaw)) * glm::cos(glm::radians(pitch)),
			glm::sin(glm::radians(pitch)),
			glm::sin(glm::radians(yaw)) * glm::cos(glm::radians(pitch))
			);
		auto cameraFront = glm::normalize(Front);


		this->m_camera->setLookAt(
			this->m_camera->getWorldTransform()[3],
			this->m_camera->getWorldTransform()[3] + glm::vec4(cameraFront, 1),
			glm::vec3(0, 1, 0));

		/*	auto YRot = glm::mat4(
		glm::vec4(cosf(deltaMouse.x * deltaTime), 0, -sinf(deltaMouse.x * deltaTime), 0),
		glm::vec4(0, 1, 0, 0),
		glm::vec4(sinf(deltaMouse.x * deltaTime), 0, cosf(deltaMouse.x * deltaTime), 0),
		glm::vec4(0, 0, 0, 1)
		);

		auto XRot = glm::mat4(
		glm::vec4(1, 0, 0, 0),
		glm::vec4(0, cosf(deltaMouse.y * deltaTime), sinf(deltaMouse.y * deltaTime), 0),
		glm::vec4(0, -sinf(deltaMouse.y * deltaTime), cos(deltaMouse.y * deltaTime), 0),
		glm::vec4(0, 0, 0, 1)
		);

		loadObjTransform = loadObjTransform * YRot;*/
	}

	if (glfwGetMouseButton(Application::_window, 1) == true) //MOUSE CLICKED
	{
		m_camera->setLookAt(m_camera->getWorldTransform()[3], glm::vec4(0), glm::vec3(0, 1, 0));
	}

	if (glfwGetKey(Application::_window, GLFW_KEY_ESCAPE))
	{
		Application::shutdown();
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
		m_camera->setLookAt(m_camera->getWorldTransform()[3], sphereTransform[3], glm::vec3(0, 1, 0));
	}

	auto sphereTranslation = glm::mat4(
		glm::vec4(1, 0, 0, 0),
		glm::vec4(0, 1, 0, 0),
		glm::vec4(0, 0, 1, 0),
		glm::vec4(50, 20, 50, 1)
		);
	sphereTransform = sphereTranslation;

	auto loadOBJTranslation = glm::mat4(
		glm::vec4(1, 0, 0, 0),
		glm::vec4(0, 1, 0, 0),
		glm::vec4(0, 0, 1, 0),
		glm::vec4(90, 0, 90, 1)
		);
	loadObjTransform = loadOBJTranslation;
}

void RenderingGeometryApp::draw()
{
	auto projView = m_camera->getProjectionView();

#pragma region Plane
	m_defaultShader->bind();
	auto projectionviewUniform = m_defaultShader->getUniform("worldViewProjection"); //GET HANDLE FOR THE UNIFORM MAT4 FOR THE WORLDVIEW MATRIX
	glUniformMatrix4fv(projectionviewUniform, 1, GL_FALSE, glm::value_ptr(projView * planeTransform)); // SEND THE SHADER PROGRAM A MODELVIEWPROJECTION MATRIX
	m_plane->draw(GL_TRIANGLES);
	m_defaultShader->unbind();
#pragma endregion 

#pragma region Cube
	m_defaultShader->bind();
	auto cProjectionViewUniform = m_defaultShader->getUniform("worldViewProjection");
	glUniformMatrix4fv(cProjectionViewUniform, 1, GL_FALSE, glm::value_ptr(projView * cubeTransform));
	m_cube->draw(GL_TRIANGLES);
	m_defaultShader->unbind();
#pragma endregion

#pragma region Sphere
	m_defaultShader->bind();
	auto sProjectionViewUniform = m_defaultShader->getUniform("worldViewProjection"); //GET HANDLE FOR THE UNIFORM MAT4 FOR THE WORLDVIEW MATRIX
	glUniformMatrix4fv(sProjectionViewUniform, 1, GL_FALSE, glm::value_ptr(projView * sphereTransform)); // SEND THE SHADER PROGRAM A MODELVIEWPROJECTION MATRIX	
	m_sphere->draw(GL_TRIANGLE_STRIP);
	m_defaultShader->unbind();
#pragma endregion

#pragma region LoadedObject
	m_textureShader->bind();
	m_texture->bind();
	auto loadProjectionViewUniform = m_textureShader->getUniform("worldViewProjection");
	//auto loadDiffuseUniform = m_textureShader->getUniform("diffuse");
	glUniformMatrix4fv(loadProjectionViewUniform, 1, GL_FALSE, glm::value_ptr(projView * loadObjTransform));
	//glUniform1i(loadDiffuseUniform, 0);
	m_loadOBJ->draw(GL_TRIANGLES);
	m_textureShader->unbind();
#pragma endregion

}
