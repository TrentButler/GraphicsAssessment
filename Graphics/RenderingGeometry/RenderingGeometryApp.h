#pragma once
#include <Application.h>

class Shader;
class Mesh;
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

	Mesh* m_object;
	Shader* m_shader;
};