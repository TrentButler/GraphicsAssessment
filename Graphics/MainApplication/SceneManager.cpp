#include "SceneManager.h"
#include <CameraApp.h>
#include <RenderingGeometryApp.h>
#include <LightingApp.h>
#include <TextureApplication.h>

SceneManager::SceneManager() {};
SceneManager::~SceneManager() {};

void SceneManager::ChangeScene(Scene scene, Application* app)
{
	//CREATE A NEW 'SCENE' BY LOADING UP AN APP
	//'HAND-OFF' THE NEW APP'S INFO TO THE OLD APP

	unsigned int appWidth = 900;
	unsigned int appHeight = 600;
	//bool appFullscreen = Application::_fullscreen;

	switch (scene)
	{
	case MAIN:
		break;

	case CAMERA:
	{
		Application* newApp = new CameraApp(); //MAKE A NEW APP
		newApp->run("camera app", appWidth, appHeight, false);
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