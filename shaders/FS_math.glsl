// the fragment shader calculates the final color output

#version 330 core
in vec2 TexCoord;

out vec4 color; // RGBA 0.0f to 1.0f

uniform sampler2D ourTexture1;
uniform sampler2D ourTexture2;
uniform sampler2D ourTexture3;

void main()
{
    color = (0.11*texture(ourTexture1, TexCoord) + 0.55*texture(ourTexture2, TexCoord) + 0.33*texture(ourTexture3, TexCoord));
}