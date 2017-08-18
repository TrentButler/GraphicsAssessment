#include "Application.h"

Application::Application() {};
Application::~Application() {};


void Application::startup() {};


void Application::shutdown() 
{
	glfwDestroyWindow(_window);
	glfwTerminate();
}


void Application::update(float time) {};


void Application::draw() {};


void Application::run(const char* title, unsigned int width, unsigned int height, bool fullscreen)
{
	if (glfwInit() == false) //INITILIZE DEPENDENCY 'glfw3', IF INITIALIZATION IS UNSUCCESSFUL TERMINATE PROGRAM
	{
		return; //TERMINATE THE PROGRAM
	}

	if (title != nullptr && width + height > 0) //NULL CHECK 'title', CHECK IF 'width' + 'height' IS GREATER THAN ZERO
	{
		if (fullscreen == true) //FULLSCREEN CHECK
		{
			_window = glfwCreateWindow(width, height, title, _monitor, nullptr); //CREATE A WINDOW
		}

		if (fullscreen == false) //FULLSCREEN CHECK
		{
			_window = glfwCreateWindow(width, height, title, nullptr, nullptr); //CREATE A WINDOW
		}
		
		if (_window == nullptr)
		{
			this->shutdown();
			return;
		}

		glfwMakeContextCurrent(_window); //GIVE THE WINDOW CONTEXT

		if (ogl_LoadFunctions() == ogl_LOAD_FAILED) // LOAD THE OPENGL FUNCTIONS, IF OPERATION IS UNSUCCESSFUL TERMINATE PROGRAM
		{
			this->shutdown(); //APPLICATION SPECIFIC SHUTDOWN METHOD, TERMINATE THE PROGRAM
			return; //TERMINATE PROGRAM
		}

		glClearColor(0.0f, 1.0f, 0.0f, 0.6f); //SET A CLEAR COLOR FOR THE BACKGROUND

		startup(); //CHILD CLASS METHOD IMPLEMENTATION

		while (glfwWindowShouldClose(_window) == GL_FALSE) //LOOP UNTIL 'glfwWindowShouldClose() == GL_TRUE'
		{
			glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); //CLEAR THE WINDOW

			float deltaTime = 0; //USED TO CALCULATE DELTATIME
			update(deltaTime); //CHILD CLASS METHOD IMPLEMENTATION
			draw(); //CHILD CLASS METHOD IMPLEMENTATION

			glfwSwapBuffers(_window); //SWAP THE FRONT AND BACK BUFFERS
			glfwPollEvents(); //CHECK FOR ANY EVENTS
		}

		this->shutdown(); //APPLICATION SPECIFIC SHUTDOWN METHOD, TERMINATE THE PROGRAM
	}

}