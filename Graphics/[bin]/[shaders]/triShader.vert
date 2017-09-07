#version 410
layout(location = 0) in vec4 Position;
layout(location = 1) in vec4 Color;
out vec4 vertexColor;
uniform mat4 wvProjection;
void main()
{
vertexColor = Color;
gl_Position = wvProjection * Position;
}