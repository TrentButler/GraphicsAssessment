#pragma once
#include <Application.h>
#include <Camera.h>
#include "Shader.h"
#include "Mesh.h"

class RenderApp : public Application
{
public:
	RenderApp();
	~RenderApp();

	void startup();
	void shutdown();
	void update(float deltaTime);
	void draw();

	void genGrid(unsigned int rows, unsigned int cols);	

private:

	Camera* _camera;
	Shader* _shader;
	Mesh* _aObject;
};