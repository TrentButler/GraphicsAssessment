#version 410
layout(location = 0) in vec4 Position;
layout(location = 1) in vec4 Color;
layout(location = 2) in vec4 Normal;
layout(location = 3) in vec3 TextureCoord;
layout(location = 4) in vec4 Tangent;

out vec4 vertexPosition;
out vec4 vertexColor;
out vec4 vertexNormal;
out vec3 vertexTextureCoord;
out vec3 vertexTangent;
out vec3 vertexBiTangent;

uniform sampler2D perlinMap;

uniform mat4 WVP;

void main()
{
vertexPosition = Position;
vertexColor = Color;
vertexNormal = Normal;
vertexTextureCoord = TextureCoord;
vertexTangent = Tangent.xyz;
vertexBiTangent = cross(vertexNormal.xyz, vertexTangent);


//DO NOISE HERE
vec4 position = Position;

//position.y += texture(perlinMap, TextureCoord.xy).r * 1.5;

gl_Position = WVP * position;
}