#include "TextureApplication.h"

int main()
{
	Application* app = new TextureApplication();
	app->run("texture app", 1000, 1000, false);
	return 1;
}