// the fragment shader calculates the final color output

#version 330 core
in vec2 TexCoord;
in float useTex;

out vec4 color; // RGBA 0.0f to 1.0f

uniform float mag;
uniform float chSelect;
uniform sampler2D braintex;
uniform sampler2D activitytex;

uniform sampler2D ch1;
uniform sampler2D ch2;
uniform sampler2D ch3;

void main()
{
	if (useTex == 2.0)
	{
		if (chSelect == 0)
			color = texture(ch1,TexCoord);
		else if (chSelect == 1)
			color = texture(ch2,TexCoord);
		else if (chSelect == 2)
			color = texture(ch3,TexCoord);
	}	
	else if (useTex == 1.0)
	{
		color = vec4(texture(braintex,TexCoord).rgb,1.0);
	}
	else
	{
		vec4 tmp2 = texture(activitytex,TexCoord);
		color = vec4(tmp2.rgb,tmp2.a*mag);
	}
}