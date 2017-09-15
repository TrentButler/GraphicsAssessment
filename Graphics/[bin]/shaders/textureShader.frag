#version 410
in vec2 vertexTextureCoord;
in vec4 vertexColor;
out vec4 fragColor;
uniform sampler2D diffuse;
void main()
{
vec4 vTexture = texture(diffuse, vertexTextureCoord);
fragColor = max(vec4(0), vTexture);
}