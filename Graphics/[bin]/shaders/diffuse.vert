#version 410
layout(location = 0) in vec4 Position;
layout(location = 1) in vec4 Color;
layout(location = 2) in vec4 Normal;
layout(location = 3) in vec3 TextureCoord;
out vec4 vertexPosition;
out vec4 vertexColor;
out vec4 vertexNormal;
out vec3 vertexTextureCoord;
uniform mat4 WVP;
void main()
{
vertexPosition = Position;
vertexColor = Color;
vertexNormal = Normal;
vertexTextureCoord = TextureCoord;
gl_Position = WVP * Position;
}