#version 410
in vec4 vertexColor;
in vec4 vertexNormal;
in vec3 vertexTextureCoord;
out vec4 fragColor;
void main()
{
fragColor = vertexColor;
}