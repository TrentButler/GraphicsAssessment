#include "SceneManager.h"
#include <CameraApp.h>
#include <RenderingGeometryApp.h>
#include <LightingApp.h>
#include <TextureApplication.h>

SceneManager::SceneManager() {};
SceneManager::~SceneManager() {};


void SceneManager::ChangeScene(Scene scene, Application* app)
{
	unsigned int appWidth = Application::_width;
	unsigned int appHeight = Application::_height;
	//bool appFullscreen = Application::_fullscreen;

	switch (scene)
	{
	case MAIN:
		break;

	case CAMERA:
	{
		Application* newApp = new CameraApp(); //MAKE A NEW APP
		app = newApp;
		app->run("camera app", appWidth, appHeight, false);
		break;
	}

	case RENDERINGGEOMETRY:
		break;

	case LIGHTING:
		break;

	case TEXTURES:
		break;
	}
}

#pragma region NOTUSED
void SceneManager::startup()
{
}
void SceneManager::shutdown()
{
}
void SceneManager::update(float time)
{
}
void SceneManager::draw()
{
}
void SceneManager::OnGUI()
{
}
#pragma endregion