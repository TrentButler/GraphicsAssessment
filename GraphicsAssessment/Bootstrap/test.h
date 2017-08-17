#pragma once
//#include "glew.h";
#include "glfw3.h";


class test
{

public:

	test(int width, int height) 
	{
		glfwInit();
		this->_window = glfwCreateWindow(width, height, "test", NULL, NULL);
		glfwMakeContextCurrent(this->_window);
		
	}

	void AppLoop()
	{
		while (!glfwWindowShouldClose(this->_window))
		{
			//DO STUFF
			glClear(GL_DEPTH_BUFFER_BIT);
			glfwSwapBuffers(this->_window);
			glfwPollEvents();
		}
	}

private:

	GLFWwindow* _window;
};
