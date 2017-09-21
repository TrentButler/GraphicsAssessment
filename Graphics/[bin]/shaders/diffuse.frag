#version 410
in vec4 vertexPosition;
in vec4 vertexColor;
in vec4 vertexNormal;
in vec3 vertexTextureCoord;

out vec4 fragColor;

uniform vec3 lightDirection;
uniform vec3 lightColor;

void main()
{
float diffuse = max(0, dot(normalize(vertexNormal.xyz), lightDirection));
vec3 lighting = vec3(lightColor * diffuse + vertexColor.xyz);
fragColor = vec4(lighting, 1);
}