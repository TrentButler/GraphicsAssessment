#pragma once
#include <vector>

class Texture
{
public:
	Texture();
	~Texture();

	void load2D(const char* path, const char* name);
	void generate2D(unsigned int width, unsigned int height, std::vector<float> data);
	void loadCubeMap(const char* path, const char* name);
	void bind(unsigned int activeTexture, unsigned int textureType);

private:
	unsigned int m_texture;
};