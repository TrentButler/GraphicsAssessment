#include "A1US_Application.h"

int main()
{
	Application* app = new App();
	app->run("A1US_Application", 1000, 600, false);
	
	return 1;
}