#include "LightingApp.h"


int main()
{
	Application* app = new LightingApp();
	app->run("lighting app", 1920, 1080, true);
	return 1;
}