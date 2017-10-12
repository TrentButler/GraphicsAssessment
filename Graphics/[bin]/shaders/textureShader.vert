#version 410
layout(location = 0) in vec4 Position; //VERTEX INFORMATION
layout(location = 1) in vec4 Color; //VERTEX INFORMATION
layout(location = 2) in vec4 Normal; //VERTEX INFORMATION
layout(location = 3) in vec3 TextureCoord; //VERTEX INFORMATION
layout(location = 4) in vec4 Tangent; //VERTEX INFORMATION

out vec4 vertexPosition;
out vec4 vertexColor;
out vec4 vertexNormal;
out vec3 vertexTextureCoord;
out vec3 vertexTangent;
out vec3 vertexBiTangent;

uniform mat4 WVP;

void main()
{
vertexPosition = Position;
vertexColor = Color;
vertexNormal = Normal;
vertexTextureCoord = TextureCoord;
vertexTangent = Tangent.xyz;
vertexBiTangent = cross(vertexNormal.xyz, vertexTangent);
gl_Position = WVP * Position;
}