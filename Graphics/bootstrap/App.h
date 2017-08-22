#pragma once
#include "Application.h"

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
};