#include "App.h"

int main()
{
	Application* app = new App();
	app->run("app", 1000, 600, false);
}