#include "CameraApp.h"

int main()
{
	Application* app = new CameraApp();
	app->run("app", 1200, 600, false);
}