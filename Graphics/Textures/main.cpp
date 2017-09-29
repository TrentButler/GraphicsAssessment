#include "TextureApplication.h"

int main()
{
	Application* app = new TextureApplication();
	app->run("texture app", 900, 600, true);
	return 1;
}