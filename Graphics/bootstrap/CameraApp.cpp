#include "CameraApp.h"
#include "FPSCamera.h"
#include <glfw3.h>
#include <glm.hpp>
#include "Gizmos.h"

CameraApp::CameraApp() {};
CameraApp::~CameraApp() {};
void CameraApp::run(const char* title, unsigned int width, unsigned int height, bool fullscreen) {};

void CameraApp::startup()
{
	//INITLIZE CAMERA
	this->_camera = new FPSCamera(); //INITILIZE CAMERA
	this->_camera->setLookAt(glm::vec3(0.1f, 0.1f, -10.0f), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)); //ORIENT THE CAMERA
	this->_camera->setPerspective(3.14f / 4.0f, 16.0f / 9.0f, 0.1f, 100.0f); //DEFINE THE CAMERA'S PROJECTION
	//this->_camera->setOrtho(0.0f, 1.50f, 1.50f, 0.0f, 100.0f, 0.1f);

	Gizmos::create();
}

void CameraApp::shutdown() {};

static double prevMouseX = 0; //USED TO CALCULATE THE MOUSE DELTA X
static double prevMouseY = 0; //USED TO CALCULATE THE MOUSE DELTA Y
static double yaw = 0; //VARIABLE USED TO STORE THE 'YAW' AMOUNT
static double pitch = 0; //VARIABLE USED TO STORE THE 'PITCH' AMOUNT
static glm::vec2 deltaMouse = glm::vec2(0); //VECTOR2 TO STORE THE CHANGE IN MOUSE POSITION
glm::mat4 sphere = glm::mat4(1); //TRANSFORM TO DEFINE AN SPHERE
glm::mat4 biggerSphere = glm::mat4(1); //TRANSFORM TO DEFINE AN SPHERE
void CameraApp::update(float deltaTime)
{
	//NEEDS WORK
	//LOOK AROUND
	//DERIVE A DELTA FROM MOUSE MOVEMENT
	static bool pressed = false;	
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


		this->_camera->setLookAt( 
			this->_camera->getWorldTransform()[3],
			this->_camera->getWorldTransform()[3] + glm::vec4(cameraFront, 1),
			glm::vec3(0, 1, 0));

		/*auto YRot = glm::mat4(
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

		auto World = this->_camera->getWorldTransform();
		auto newWorld = World * YRot;
		//auto newWorld = YRot * World;
		this->_camera->setWorldTransform(newWorld);*/
	}

	//MOVE CAMERA
	if (glfwGetKey(Application::_window, GLFW_KEY_W))
	{
		//MOVE FOWARD (-Z)
		//GET PREVIOUS POSITION, TRANSLATE, THEN SET IT
		auto Previous = this->_camera->getWorldTransform();
		auto Translation = glm::mat4(glm::vec4(1, 0, 0, 0), glm::vec4(0, 1, 0, 0),
			glm::vec4(0, 0, 1, 0), glm::vec4(0, 0, -4 * deltaTime, 1));

		//auto newPositon = (Previous * Translation)[3];
		//this->_camera->setPosition(newPositon);
		auto newWorld = Previous * Translation;
		this->_camera->setWorldTransform(newWorld);
	}

	if (glfwGetKey(Application::_window, GLFW_KEY_S))
	{
		//MOVE BACKWARD (+Z)
		auto Previous = this->_camera->getWorldTransform();
		auto Translation = glm::mat4(glm::vec4(1, 0, 0, 0), glm::vec4(0, 1, 0, 0),
			glm::vec4(0, 0, 1, 0), glm::vec4(0, 0, 4 * deltaTime, 1));

		//auto newPositon = (Previous * Translation)[3];
		//this->_camera->setPosition(newPositon);
		auto newWorld = Previous * Translation;
		this->_camera->setWorldTransform(newWorld);
	}

	if (glfwGetKey(Application::_window, GLFW_KEY_A))
	{
		//MOVE LEFT (-X)
		auto Previous = this->_camera->getWorldTransform();
		auto Translation = glm::mat4(glm::vec4(1, 0, 0, 0), glm::vec4(0, 1, 0, 0),
			glm::vec4(0, 0, 1, 0), glm::vec4(-4 * deltaTime, 0, 0, 1));

		//auto newPositon = (Previous * Translation)[3];
		//this->_camera->setPosition(newPositon);
		auto newWorld = Previous * Translation;
		this->_camera->setWorldTransform(newWorld);
	}

	if (glfwGetKey(Application::_window, GLFW_KEY_D))
	{
		//MOVE RIGHT (+X)
		auto Previous = this->_camera->getWorldTransform();
		auto Translation = glm::mat4(glm::vec4(1, 0, 0, 0), glm::vec4(0, 1, 0, 0),
			glm::vec4(0, 0, 1, 0), glm::vec4(4 * deltaTime, 0, 0, 1));

		//auto newPositon = (Previous * Translation)[3];
		//this->_camera->setPosition(newPositon);
		auto newWorld = Previous * Translation;
		this->_camera->setWorldTransform(newWorld);
	}


	//CHANGE CURRENT PROJECTION
	if (glfwGetKey(Application::_window, GLFW_KEY_F1))
	{
		//CHANGE PROJECTION
		this->_camera->setOrtho(0.0f, 2.0f, 2.0f, 0.0f, 100.0f, 0.1f);

	}

	if (glfwGetKey(Application::_window, GLFW_KEY_F2))
	{
		this->_camera->setPerspective(3.14f / 4.0f, 16.0f / 9.0f, 0.1f, 100.0f);
	}

	//LOOK AT A OBJECT
	if (glfwGetKey(Application::_window, GLFW_KEY_1))
	{
		this->_camera->setLookAt(this->_camera->getView()[3], sphere[3], glm::vec3(0, 1, 0));
	}

	if (glfwGetKey(Application::_window, GLFW_KEY_2))
	{
		this->_camera->setLookAt(this->_camera->getView()[3], biggerSphere[3], glm::vec3(0, 1, 0));
	}

	auto Translation = glm::mat4(
		glm::vec4(1, 0, 0, 0),
		glm::vec4(0, 1, 0, 0),
		glm::vec4(0, 0, 1, 0),
		glm::vec4(10, 0, -12, 1));
	biggerSphere = sphere * Translation;

	//this->_camera->setLookAt(this->_camera->getView()[3], biggerSphere[3], glm::vec3(0, 1, 0));

}


void CameraApp::draw()
{
	Gizmos::clear();

	Gizmos::addSphere(sphere[3], 1, 10, 10, glm::vec4(0.0f, 1.0f, 0.0f, 0.4f), &sphere);
	Gizmos::addTransform(sphere, 4);

	Gizmos::addSphere(biggerSphere[3], 2, 10, 10, glm::vec4(0.0f, 0.0f, 1.0f, 0.4f), &biggerSphere);
	Gizmos::addTransform(sphere, 4);

	//Gizmos::addTri(glm::vec3(-1, 0, 0), glm::vec3(0, 1, 0), glm::bvec3(1, 0, 0), glm::vec4(0.0f, 0.8f, 0.0f, 0.6f));


	Gizmos::draw(this->_camera->getProjectionView());
}