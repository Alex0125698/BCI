// the purpose of the vertex shader is to transform 3D coordinates into different 3D coordinates
// Note: it does not do interpolation - it only operates on the vertices

#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;

// texture position (0 for non-electrode)
uniform vec2 pos;
// texture size
uniform float size;
// aspectRatio = width / height
uniform float aspectRatio; 

out vec2 TexCoord;
//!out vec4 gl_Position;

void main()
{
	TexCoord = texCoord;

	float x = position.x*size+pos.x;
	float y = position.y*size+pos.y;

	gl_Position = vec4(x, y, position.z, 1.0);
}