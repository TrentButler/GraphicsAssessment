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
	Mesh* m_sphere;
	Mesh* m_perlinMesh;

	Shader* m_shader;
	Shader* m_multTexShader;
	Shader* m_perlinShader;

	
	Texture* m_diffuseMap;
	Texture* m_normalMap;
	Texture* m_animatedTexture;
	Texture* m_perlinTexture;
};