#version 410
in vec4 vertexPosition;
in vec4 vertexColor;
in vec4 vertexNormal;
in vec3 vertexTextureCoord;

out vec4 fragColor;

uniform vec3 lightDirection;
uniform vec3 lightColor;
uniform vec3 cameraPosition;
uniform float specularPower;

uniform vec3 upVector;
uniform vec3 skyColor;
uniform vec3 groundColor;

void main()
{
float vertNormDotUp = dot(vertexNormal.xyz, upVector);
float influence = vertNormDotUp + (0.5f * 0.5f);
vec3 hemisphereLighting = mix(groundColor, skyColor, influence);

float diffuse = max(0, dot(normalize(vertexNormal.xyz), lightDirection));
vec3 E = normalize(cameraPosition - vertexPosition.xyz);
vec3 R = reflect(-lightDirection, vertexNormal.xyz);
float specular = max(0, dot(E, R));
specular = pow(specular, specularPower);

vec3 lighting = vec3(lightColor * diffuse + lightColor * specular + vertexColor.xyz);

fragColor = vec4(lighting, 1);
}