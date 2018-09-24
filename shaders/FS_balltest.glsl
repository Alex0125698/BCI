// the fragment shader calculates the final color output

#version 330 core
in vec2 TexCoord;

out vec4 color; // RGBA 0.0f to 1.0f

uniform sampler2D balltex;

void main()
{
    color = texture(balltex, TexCoord);
}