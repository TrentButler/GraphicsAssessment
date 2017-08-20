#include "A1US_Application.h"


void App::shutdown() {};
void App::update(float time) {};
void App::run(const char* title, unsigned int width, unsigned int height, bool fullscreen) {};


void App::startup()
{
	Gizmos::create(); //INITILIZE DEPENDENCY
	this->_view = glm::lookAt(glm::vec3(0, 10, -10), glm::vec3(0), glm::vec3(0, 1, 0)); //DEFINE THE CAMERA'S VIEW
	this->_projection = glm::perspective(glm::radians(45.0f), 16.0f / 9.0f, 100.0f, 0.1f); //DEFINE THE CAMERA'S PERSPECTIVE
};

void App::draw() 
{
	Gizmos::addTransform(glm::mat4(1));
	Gizmos::addTri(glm::vec3(0, 1, 0), glm::vec3(-1, 0, 0), glm::vec3(1, 0, 0), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));

	//Gizmos::addSphere()

	Gizmos::draw(this->_projection * this->_view);

	Gizmos::clear();
}