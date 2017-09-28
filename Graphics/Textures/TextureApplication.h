#pragma once
#include <Application.h>

struct Mesh;
struct Shader;
struct Camera;
struct Texture;
class TextureApplication : public Application
{
public:
	TextureApplication();
	~TextureApplication();

	void startup();

	void shutdown();

	void update(float deltaTime);

	void draw();


	// Inherited via Application
	virtual void OnGUI() override;


private:
	Camera* m_camera;
	Mesh* m_plane;
	Shader* m_shader;
	Texture* m_texture;

};