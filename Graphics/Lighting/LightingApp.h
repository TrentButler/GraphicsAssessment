#pragma once
#include <Application.h>
#include <vector>

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

	Mesh* m_bunny;

	Shader* m_shader;
	Shader* m_ambient;
	Shader* m_diffuse;
	Shader* m_specular;
	Shader* m_blinn;
	Shader* m_phong;

};