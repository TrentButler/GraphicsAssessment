#include "RenderingGeometryApp.h"


int main()
{
	Application* app = new RenderingGeometryApp();
	app->run("rendering geometry", 1000, 600, false);

	return 1;
}