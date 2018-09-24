// the purpose of the vertex shader is to transform 3D coordinates into different 3D coordinates
// Note: it does not do interpolation - it only operates on the vertices

#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;
uniform vec2 pos;
uniform float aspectRatio; // = width / height
out vec2 TexCoord;

//!out vec4 gl_Position;

void main()
{
	TexCoord = vec2(texCoord.x, texCoord.y);

	float x = position.x * 0.09 + pos.x;
	float y = position.y * 0.09 * aspectRatio + pos.y;
	float z = position.z;
	float w = 1.0;

	gl_Position = vec4(x,y,z,w);
}