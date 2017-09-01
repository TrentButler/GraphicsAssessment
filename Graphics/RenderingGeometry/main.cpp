#include <windows.h>
#include <iostream>
#include "RenderApp.h"
#include "Shader.h"

int main()
{
	Application* app = new RenderApp();
	app->run("rendering geometry", 1200, 600, false);

	return 1;
}