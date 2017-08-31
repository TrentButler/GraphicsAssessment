#pragma once

class Shader
{
public:
	Shader();
	~Shader();

	void LoadShader(const char* vert, const char* frag);
	unsigned int getShaderProgID();

private:
	unsigned int _programID;
};