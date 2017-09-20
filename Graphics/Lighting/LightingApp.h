#pragma once
#include <Application.h>

struct Mesh;
struct Shader;
struct Texture;
struct Camera;
class LightingApp : public Application
{

public:

	LightingApp();
	~LightingApp();
	
	void startup();

	void shutdown();

	void update(float time);

	void draw();

private:

	Camera* m_camera;
	Mesh* m_plane;
	Mesh* m_sphere;
	Mesh* m_loadOBJ;
	Shader* m_shader;
	Shader* m_lighting;
};