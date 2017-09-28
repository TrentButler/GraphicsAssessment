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

uniform vec3 upVector;
uniform vec3 skyColor;
uniform vec3 groundColor;

void main()
{
float vertNormDotUp = dot(vertexNormal.xyz, upVector);
float influence = vertNormDotUp + (0.5f * 0.5f); //CALCULATE A LIGHTING INFLUENCE USING THE DOT PRODUCT OF THE VERTEX NORMAL AND A UP VECTOR
vec3 Ambient = mix(groundColor, skyColor, influence); //USE 'influence' TO MIX THE 'SKY COLOR' AND THE 'GROUND COLOR',

float diffuse = max(0, dot(normalize(vertexNormal.xyz), lightDirection)); //DIFFUSE CALCULATION

vec3 viewDirection = normalize(-cameraPosition.xyz); //CALCULATE THE CAMERA'S EYE DIRECTION
vec3 lightDir = normalize(lightDirection - vertexPosition.xyz); //CALCULATE THE LIGHT'S DIRECTION

vec3 halfwayVector = normalize(lightDir + viewDirection); //HALFWAY VECTOR CALCULATION
float specular = max(dot(halfwayVector, normalize(vertexNormal.xyz)), 0); //BLINN-PHONG SPECULAR VALUE CALCULATION

float SPECULARPOWER = 16.0f; //USE A LOWER SPECULAR POWER FOR BLINN PHONG
specular = pow(specular, SPECULARPOWER); //RAISE THE SPECULAR VALUE BY 'SPECULARPOWER'

vec3 Diffuse = lightColor * diffuse; //MULTIPLY THE DIFFUSE VALUE BY THE LIGHTCOLOR TO GET DIFFUSE LIGHTING
vec3 Specular = lightColor * specular; //MULTIPLY THE SPECULAR VALUE BY THE LIGHTCOLOR TO GET SPECULAR LIGHTING

fragColor = vec4(Ambient + Diffuse + Specular, 1); //ADD THE AMBIENT, DIFFUSE, AND SPECULAR VALUES TOGETHER TO COLOR THE VERTEX
}