#pragma once
#include "Application.h"

struct Camera;
class CameraApp : public Application
{
public:
	CameraApp();
	~CameraApp();
	void run(const char* title, unsigned int width, unsigned int height, bool fullscreen);

protected:
	void startup();
	virtual void shutdown();
	virtual void update(float time);
	virtual void draw();

	GLFWwindow* _window;
	GLFWmonitor* _monitor;
	Camera* _camera;
};