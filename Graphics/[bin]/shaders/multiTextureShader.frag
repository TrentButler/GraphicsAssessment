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

out vec4 fragColor;

void main()
{
    
mat3 TBN = mat3(
    normalize(vertexTangent),
    normalize(vertexBiTangent),
    normalize(vertexNormal)
    );

vec3 getNormalMapTextureRGB = texture(normalMap, vertexTextureCoord.xy).xyz;
vec3 convertedNormalMapTextureRGB = getNormalMapTextureRGB * 2 - 1;
vec3 textureSurfaceNormal = TBN * convertedNormalMapTextureRGB;

float diffuse = max(0, dot(normalize(textureSurfaceNormal), normalize(lightDirection))); //DIFFUSE CALCULATION

vec3 getDiffuseMapTextureRGB = texture(diffuseMap, vertexTextureCoord.xy).xyz;

fragColor = vec4(getDiffuseMapTextureRGB * diffuse, 1);
}