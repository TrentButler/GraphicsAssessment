#version 410
layout(location = 0) in vec4 Position;
layout(location = 1) in vec4 Color;
layout(location = 2) in vec4 Normal;
layout(location = 3) in vec2 TextureCoord;
out vec4 vertexColor;
out vec2 vertexTextureCoord;
uniform mat4 worldViewProjection;
void main()
{
vertexTextureCoord = TextureCoord;
vertexColor = Color;
gl_Position = worldViewProjection * Position;
}