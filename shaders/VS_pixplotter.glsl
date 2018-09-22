// the purpose of the vertex shader is to transform 3D coordinates into different 3D coordinates
// Note: it does not do interpolation - it only operates on the vertices

#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;

uniform float range;

out vec2 TexCoord;

//!out vec4 gl_Position;

void main()
{
	TexCoord = vec2(texCoord.y*range, texCoord.x);
	gl_Position = vec4(position,1.0);
}