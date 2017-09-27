#version 410
in vec4 vertexPosition; //VERTEX INFORMATION
in vec4 vertexColor; //VERTEX INFORMATION
in vec4 vertexNormal; //VERTEX INFORMATION
in vec3 vertexTextureCoord; //VERTEX INFORMATION

out vec4 fragColor; //OUTPUT THE COLOR OF THE VERTEX

uniform vec3 upVector;
uniform vec3 skyColor;
uniform vec3 groundColor;

void main()
{
float vertNormDotUp = dot(vertexNormal.xyz, upVector);
float influence = vertNormDotUp + (0.5f * 0.5f); //CALCULATE A LIGHTING INFLUENCE USING THE DOT PRODUCT OF THE VERTEX NORMAL AND A UP VECTOR

vec3 hemisphereLighting = mix(groundColor, skyColor, influence); //USE 'influence' TO MIX THE 'SKY COLOR' AND THE 'GROUND COLOR'
fragColor = vec4(hemisphereLighting, 1); //COLOR THE VERTEX WITH THE CALCULATED LIGHTING
}