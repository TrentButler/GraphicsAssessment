#include "TextureApplication.h"
#include <CameraApp.h>

int main()
{
	Application* app = new TextureApplication();
	app->run("texture app", 900, 600, false);
	return 1;
}