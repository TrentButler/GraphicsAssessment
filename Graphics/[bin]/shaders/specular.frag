#version 410
in vec4 vertexPosition; //VERTEX INFORMATION
in vec4 vertexColor; //VERTEX INFORMATION
in vec4 vertexNormal; //VERTEX INFORMATION
in vec3 vertexTextureCoord; //VERTEX INFORMATION

out vec4 fragColor;

uniform vec3 lightDirection;
uniform vec3 lightColor;
uniform vec3 cameraPosition;
uniform float specularPower;

void main()
{
vec3 E = normalize(cameraPosition - vertexPosition.xyz); // VEC3 REPRENSENTATION OF THE VIEW DIRECTION
vec3 R = reflect(-lightDirection, vertexNormal.xyz); //CALCULATE A REFLECTION DIRECTION
float specular = max(0, dot(E, R)); //CALCULATE THE SPECULAR VALUE
specular = pow(specular, specularPower); //RAISE THE SPECULAR VALUE BY 'specularPower'

vec3 lighting = vec3(lightColor * specular + vertexColor.xyz); //MULTIPLY THE LIGHT COLOR BY THE SPECULAR VALUE, THEN ADD THE VERTEX COLOR

fragColor = vec4(lighting, 1); //COLOR THE VERTEX
}