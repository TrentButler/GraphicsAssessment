#include "MainApp.h"

int main()
{
	Application* app = new MainApp();
	app->run("main app", 900, 600, false);
	return 1;
}