#pragma once

class Shader
{
public:
	Shader();
	~Shader();

	void bind();
	void unbind();
	void LoadShader(const char* vert, const char* frag);
	void attach();
	unsigned int getUniform(const char* varName);

private:
	unsigned int _programID;
	unsigned int vertID;
	unsigned int fragID;
};