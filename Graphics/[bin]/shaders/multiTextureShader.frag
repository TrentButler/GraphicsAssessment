#version 410
in vec4 vertexPosition; //VERTEX INFORMATION
in vec4 vertexColor; //VERTEX INFORMATION
in vec4 vertexNormal; //VERTEX INFORMATION
in vec3 vertexTextureCoord; //VERTEX INFORMATION
in vec3 vertexTangent;
in vec3 vertexBiTangent;

uniform vec3 lightDirection;

uniform sampler2D diffuseMap;
uniform sampler2D normalMap;

uniform float time;
out vec4 fragColor;

void main()
{
    
mat3 TBN = mat3(
    normalize(vertexTangent),
    normalize(vertexBiTangent),
    normalize(vertexNormal)
    );

vec2 UV = vertexTextureCoord.xy;

UV.y -= time * 0.05;

vec3 normalTexture = texture(normalMap, UV.xy).xyz;

vec3 convertedNormalMapTextureRGB = normalTexture * 2 - 1;

vec3 N = normalize(TBN * convertedNormalMapTextureRGB);
vec3 L = normalize(lightDirection);
vec3 SN = normalize(vertexNormal.xyz);
float diffuseTerm = max(0, dot(N, -L)); //DIFFUSE CALCULATION

vec3 diffuseLighting = vec3(0.2) * diffuseTerm;

vec3 diffuseTexture = texture(diffuseMap, UV.xy).xyz;

fragColor = vec4(diffuseTexture + diffuseLighting, 1);

}