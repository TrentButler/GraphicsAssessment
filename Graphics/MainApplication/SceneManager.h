#pragma once
#include <Application.h>

enum Scene
{
	MAIN = 0,
	CAMERA = 1,
	RENDERINGGEOMETRY = 2,
	LIGHTING = 3,
	TEXTURES = 4
};

class SceneManager : public Application
{
public:
	SceneManager();
	~SceneManager();
	void ChangeScene(Scene scene, Application* app);

#pragma region NOTUSED
	virtual void startup() override;
	virtual void shutdown() override;
	virtual void update(float time) override;
	virtual void draw() override;
	virtual void OnGUI() override;
#pragma endregion
};