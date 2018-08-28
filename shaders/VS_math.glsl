// the purpose of the vertex shader is to transform 3D coordinates into different 3D coordinates
// Note: it does not do interpolation - it only operates on the vertices

#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;

out vec2 TexCoord;

uniform mat4 view;
//uniform mat4 model;
//uniform mat4 view;
//uniform mat4 proj;

//!out vec4 gl_Position;

void main()
{
	//gl_Position = vec4(vec3(proj * view * model * vec4(position, 1.0f)).xyz, 1.0);
	TexCoord = vec2(texCoord.x, texCoord.y);
	gl_Position = view * vec4(position,1.0);
}