#include "App.h"
#include "Gizmos.h"
#include "FlyCamera.h"
#include <cmath>


//NEEDS WORK
//UPDATE THE VIEWPROJECTION MATRIX
//DEBUG IT


void App::startup()
{
	//INITLIZE CAMERA
	this->_camera = new FlyCamera();
	this->_camera->setLookAt(glm::vec3(10, 10, 10), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	this->_camera->setPerspective(3.14f / 4.0f, 16.0f/9.0f, 0.1f, 100.0f);
		
	Gizmos::create();
}

void App::shutdown() {};

void App::update(float time) 
{
	//UPDATE CAMERA 
	//this->_camera->getProjectionView();
}

void App::draw() 
{
	Gizmos::clear();
	
	Gizmos::addSphere(glm::vec3(0), 1, 20, 20, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	Gizmos::addTransform(glm::mat4(1));

	/*Gizmos::addTri(glm::vec3(-1, 0, 0), glm::vec3(0, 1, 0), glm::bvec3(1, 0, 0), glm::vec4(0.0f, 0.8f, 0.0f, 0.6f));	
	Gizmos::addTransform(glm::mat4(1));*/

	Gizmos::draw(this->_camera->getProjectionView());
	/*Gizmos::draw(glm::mat4(1));
	auto View = this->_camera->getView();*/
}

void App::run(const char* title, unsigned int width, unsigned int height, bool fullscreen) {};