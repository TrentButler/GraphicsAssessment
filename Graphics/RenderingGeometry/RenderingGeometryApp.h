#pragma once
#include <Application.h>

class Shader;
class Mesh;
class Texture;
class Camera;
class RenderingGeometryApp : public Application
{
public:
	RenderingGeometryApp();
	~RenderingGeometryApp();

	// Inherited via Application
	virtual void startup() override;

	virtual void shutdown() override;

	virtual void update(float time) override;

	virtual void draw() override;

private:

	Mesh* m_plane;
	Mesh* m_cube;
	Mesh* m_sphere;
	Mesh* m_loadOBJ;

	Shader* m_defaultShader;
	Shader* m_textureShader;
	Texture* m_texture;

	Camera* m_camera;

	// Inherited via Application
	virtual void OnGUI() override;
};