#version 410
in vec4 vertexPosition; //VERTEX INFORMATION
in vec4 vertexColor; //VERTEX INFORMATION
in vec4 vertexNormal; //VERTEX INFORMATION
in vec3 vertexTextureCoord; //VERTEX INFORMATION
in vec3 vertexTangent; //VERTEX INFORMATION
in vec3 vertexBiTangent; //VERTEX INFORMATION

uniform sampler2D diffuseMap; //VARIABLE TO HOLD THE TEXTURE INFORMATION

out vec4 fragColor;

void main()
{
vec3 diffuseTexture = texture(diffuseMap, vertexTextureCoord.xy).rrr;

fragColor = vec4(diffuseTexture + vertexColor.xyz, 1);
}