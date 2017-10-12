#pragma once
#include <Application.h>

struct Mesh;
struct Shader;
struct Camera;
struct Texture;
struct SceneManager;
class MainApp : public Application
{
public:
	MainApp();
	~MainApp();

	void startup();

	void shutdown();

	void update(float deltaTime);

	void draw();

	// Inherited via Application
	virtual void OnGUI() override;


private:
	Camera* m_camera;

	Mesh* m_sphere;
	Shader* m_shader;
	Texture* m_animatedTexture;

	SceneManager* m_manager;
};