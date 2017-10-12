#version 410
layout(location = 0) in vec4 Position; //VERTEX INFORMATION
layout(location = 1) in vec4 Color; //VERTEX INFORMATION
layout(location = 2) in vec4 Normal; //VERTEX INFORMATION
layout(location = 3) in vec3 TextureCoord; //VERTEX INFORMATION
layout(location = 4) in vec4 Tangent; //VERTEX INFORMATION

out vec4 vertexPosition; //SEND VERTEX INFO DOWN THE PIPELINE
out vec4 vertexColor; //SEND VERTEX INFO DOWN THE PIPELINE
out vec4 vertexNormal; //SEND VERTEX INFO DOWN THE PIPELINE
out vec3 vertexTextureCoord; //SEND VERTEX INFO DOWN THE PIPELINE
out vec3 vertexTangent; //SEND VERTEX INFO DOWN THE PIPELINE
out vec3 vertexBiTangent; //SEND VERTEX INFO DOWN THE PIPELINE

uniform sampler2D diffuseMap; //VARIABLE TO HOLD THE TEXTURE INFORMATION

uniform mat4 WVP;

void main()
{
vertexPosition = Position;
vertexColor = Color;
vertexNormal = Normal;
vertexTextureCoord = TextureCoord;
vertexTangent = Tangent.xyz;

vec4 position = Position; //CREATE A COPY OF 'Position' 
vec4 diffuseTexture = texture(diffuseMap, TextureCoord.xy); //LOAD THE TEXTURE

//position.x += diffuseTexture.r * 1.5;
//position.y += diffuseTexture.r * 100; //INCREMENT 'position' BY THE TEXTURES 'y' VALUE
//position.z += diffuseTexture.r * 2;

gl_Position = WVP * position; //SEND OUT THE MODIFIED POSITION;
}