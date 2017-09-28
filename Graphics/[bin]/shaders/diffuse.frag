#version 410
in vec4 vertexPosition; //VERTEX INFORMATION
in vec4 vertexColor; //VERTEX INFORMATION
in vec4 vertexNormal; //VERTEX INFORMATION
in vec3 vertexTextureCoord; //VERTEX INFORMATION

out vec4 fragColor; //OUTPUT THE COLOR OF THE VERTEX

uniform vec3 lightDirection;
uniform vec3 lightColor;

void main()
{
float diffuse = max(0, dot(normalize(vertexNormal.xyz), lightDirection)); //DIFFUSE CALCULATION
vec3 lighting = vec3(lightColor * diffuse + vertexColor.xyz); //TO GET DIFFUSE LIGHTING, MULTIPLY (diffuse) WITH THE LIGHTCOLOR AND VERTEX COLOR
fragColor = vec4(lighting, 1); //COLOR THE VERTEX WITH THE CALCULATED DIFFUSE LIGHTING
}