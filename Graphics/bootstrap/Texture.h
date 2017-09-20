#pragma once

class Texture
{
public:
	Texture();
	~Texture();

	void load(const char* path, const char* name);
	void bind();

private:
	unsigned int m_texture;
};