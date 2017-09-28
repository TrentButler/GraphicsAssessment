#include "Application.h"
#include <glfw3.h>
#include <imgui.h>
#include "imgui_impl_glfw_gl3.h"

Application::Application() {};
Application::~Application() {};

void Application::run(const char* title, unsigned int width, unsigned int height, bool fullscreen)
{
	if (glfwInit() == false) //INITILIZE DEPENDENCY 'glfw3', IF INITIALIZATION IS UNSUCCESSFUL TERMINATE PROGRAM
	{
		return; //TERMINATE THE PROGRAM
	}

	if (title != nullptr && width + height > 0) //NULL CHECK 'title', CHECK IF 'width' + 'height' IS GREATER THAN ZERO
	{
		this->_width = width;
		this->_height = height;

		if (fullscreen == true) //FULLSCREEN CHECK
		{
			this->_window = glfwCreateWindow(width, height, title, glfwGetPrimaryMonitor(), nullptr); //CREATE A WINDOW
		}

		if (fullscreen == false) //FULLSCREEN CHECK
		{
			this->_window = glfwCreateWindow(width, height, title, nullptr, nullptr); //CREATE A WINDOW
		}

		if (this->_window == nullptr)
		{
			this->shutdown();
			return;
		}

		glfwMakeContextCurrent(this->_window); //GIVE THE WINDOW CONTEXT

		if (ogl_LoadFunctions() == ogl_LOAD_FAILED) // LOAD THE OPENGL FUNCTIONS, IF OPERATION IS UNSUCCESSFUL TERMINATE PROGRAM
		{
			this->shutdown(); //APPLICATION SPECIFIC SHUTDOWN METHOD, TERMINATE THE PROGRAM
			return; //TERMINATE PROGRAM
		}

		glClearColor(0.2f, 0.2f, 0.2f, 1.0f); //SET A CLEAR COLOR FOR THE BACKGROUND

		ImGui_ImplGlfwGL3_Init(this->_window, true);
		ImGuiIO& io = ImGui::GetIO();

		io.DisplaySize.x = this->_width;
		io.DisplaySize.y = this->_height;		

		startup(); //CHILD CLASS METHOD IMPLEMENTATION

		float deltaTime = 0; //USED TO CALCULATE DELTATIME
		float prevTime = 0; //USED TO CALCULATE DELTATIME
		float currTime = 0; //USED TO CALCULATE DELTATIME

		glEnable(GL_DEPTH_TEST);

		while (glfwWindowShouldClose(this->_window) == GL_FALSE) //LOOP UNTIL 'glfwWindowShouldClose() == GL_TRUE'
		{
			glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); //CLEAR THE WINDOW			

			currTime = glfwGetTime();
			deltaTime = currTime - prevTime; //DELTATIME CALCULATION
			prevTime = currTime;

			glfwPollEvents(); //CHECK FOR ANY EVENTS
			ImGui_ImplGlfwGL3_NewFrame();

			update(deltaTime); //CHILD CLASS METHOD IMPLEMENTATION
			draw(); //CHILD CLASS METHOD IMPLEMENTATION
			OnGUI();

			ImGui::Render(); //RENDER THE UI

			glfwSwapBuffers(this->_window); //SWAP THE FRONT AND BACK BUFFERS
		}

		shutdown(); //TERMINATE THE PROGRAM
	}
}