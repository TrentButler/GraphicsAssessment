#version 410
layout(location = 0) in vec4 Position; //VERTEX INFORMATION 
layout(location = 1) in vec4 Color; //VERTEX INFORMATION
layout(location = 2) in vec4 Normal; //VERTEX INFORMATION
layout(location = 3) in vec3 TextureCoord; //VERTEX INFORMATION

out vec4 vertexPosition; 
out vec4 vertexColor; 
out vec4 vertexNormal; 
out vec3 vertexTextureCoord; 

uniform mat4 WVP; //UNIFORM VARIABLE FOR THE 'WORLD VIEW PROJECTION' MATRIX

void main(
{
vertexPosition = Position; //SEND THE VERTEX INFORMATION DOWN THE PIPELINE
vertexColor = Color; //SEND THE VERTEX INFORMATION DOWN THE PIPELINE
vertexNormal = Normal; //SEND THE VERTEX INFORMATION DOWN THE PIPELINE
vertexTextureCoord = TextureCoord; //SEND THE VERTEX INFORMATION DOWN THE PIPELINE
gl_Position = WVP * Position;
}