#pragma once
#include "Application.h"
#include <glm\glm.hpp>
#include <glm\ext.hpp>
//#include <glm\gtx\gtx.hpp>
#include <glm\gtx\projection.hpp>
#include "Gizmos.h"

class App : public Application
{

public:
	App() {}
	~App() {}
	void run(const char* title, unsigned int width, unsigned int height, bool fullscreen);

protected:
	void startup();
	virtual void shutdown();
	virtual void update(float time);
	virtual void draw();

	GLFWwindow* _window;
	GLFWmonitor* _monitor;

	//DRAWING VARIABLES
	glm::mat4 _view;
	glm::mat4 _projection;
};