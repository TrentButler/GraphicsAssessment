#version 410
in vec4 vertexPosition; //VERTEX INFORMATION
in vec4 vertexColor; //VERTEX INFORMATION
in vec4 vertexNormal; //VERTEX INFORMATION
in vec3 vertexTextureCoord; //VERTEX INFORMATION
in vec3 vertexTangent;
in vec3 vertexBiTangent;

uniform sampler2D diffuseMap;

uniform float time;

out vec4 fragColor;

void main()
{
vec2 UV = vertexTextureCoord.xy;

//UV.y -= time * 0.04;

vec3 diffuseTexture = texture(diffuseMap, UV.xy).xyz;

//fragColor = vec4(diffuseTexture, 1);
fragColor = vec4(.5,.5,.5,1);
}