// the fragment shader calculates the final color output

#version 330 core
in vec2 TexCoord;

out vec4 color; // RGBA 0.0f to 1.0f

uniform sampler2D ourTexture1;
uniform sampler2D ourTexture2;

void main()
{
    color = texture(ourTexture1, TexCoord)*texture(ourTexture2, TexCoord);
}