#version 410
in vec3 vertexTextureCoord;
in vec4 vertexColor;
out vec4 fragColor;
uniform sampler2D vertexTexture1;
uniform sampler2D vertexTexture2;
void main()
{
vec4 vTexture = texture(vertexTexture1, vertexTextureCoord.xy);
vec4 vTexture2 = texture(vertexTexture2, vertexTextureCoord.xy);
fragColor = vTexture + vTexture2;
}