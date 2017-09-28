#pragma once

class Texture
{
public:
	Texture();
	~Texture();

	void load2D(const char* path, const char* name);
	void loadCubeMap(const char* path, const char* name);
	void bind(unsigned int activeTexture, unsigned int textureType);

private:
	unsigned int m_texture;
};