#version 410
in vec3 vertexTextureCoord;
in vec4 vertexColor;
out vec4 fragColor;
uniform sampler2D vertexTexture;
void main()
{
vec4 vTexture = texture(vertexTexture, vertexTextureCoord.xy);
fragColor = vTexture;
}