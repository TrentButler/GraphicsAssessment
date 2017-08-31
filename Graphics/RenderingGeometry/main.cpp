#include <windows.h>
#include <iostream>
#include <CameraApp.h>

int main()
{
	Application* app = new CameraApp();
	app->run("rendering geometry", 1200, 600, false);

	return 1;
}