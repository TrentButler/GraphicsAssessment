#include "App.h"
#include <glm.hpp>
#include <iostream>
using namespace std;


int main()
{
	Application* app = new App();
	app->run("app", 1000, 600, false);	
}