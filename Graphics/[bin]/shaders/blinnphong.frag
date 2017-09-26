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

float vertNormDotUp = dot(normalize(vertexNormal.xyz), upVector);
float influence = vertNormDotUp + (0.5f * 0.5f);
vec3 Ambient = mix(groundColor, skyColor, influence);

float diffuse = max(0, dot(normalize(vertexNormal.xyz), lightDirection));
vec3 Diffuse = lightColor * diffuse;


vec3 viewDirection = normalize(-cameraPosition.xyz);
vec3 lightDir = normalize(lightDirection - vertexPosition.xyz);

vec3 halfwayVector = normalize(lightDir + viewDirection);
float specular = max(dot(halfwayVector, normalize(vertexNormal.xyz)), 0);

float SPECULARPOWER = 16.0f; //USE A LOWER SPECULAR POWER FOR BLINN PHONG

specular = pow(specular, SPECULARPOWER);
vec3 Specular = lightColor * specular;

fragColor = vec4(Ambient + Diffuse + Specular, 1);
}