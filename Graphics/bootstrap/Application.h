#pragma once
#include "gl_core_4_4.h"

struct GLFWmonitor;
struct GLFWwindow;
class Application
{

public:
	Application();
	~Application();
	void run(const char* title, unsigned int width, unsigned int height, bool fullscreen);

protected:
	virtual void startup() = 0;
	virtual void shutdown() = 0;
	virtual void update(float time) = 0;
	virtual void draw() = 0;

	unsigned int _width;
	unsigned int _height;

	GLFWwindow* _window;
	GLFWmonitor* _monitor;
};