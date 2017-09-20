#include "LightingApp.h"


int main()
{
	Application* app = new LightingApp();
	app->run("lighting app", 900, 600, false);
	return 1;
}