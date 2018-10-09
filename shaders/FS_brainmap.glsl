// the fragment shader calculates the final color output

#version 330 core
in vec2 TexCoord;
out vec4 color; // RGBA 0.0f to 1.0f

uniform float elecBrightness;
uniform float headBrightness;
// selects between channel names (2) ; 10-20 system (1) ; activity (else)
uniform int useTex;
// magnitude of current electrode
uniform float mag;
// is the current electrode selected?
uniform bool selected;
// switch between red (0) ; blue (1) ; off (else)
uniform int col;
// the 10-20 system texture
uniform sampler2D braintex;
// the shading aroung each electrode
uniform sampler2D activitytex;
// electrode names
uniform sampler2D channels;

void main()
{
	if (useTex == 2)
	{
		color = texture(channels,TexCoord);
		if (selected) color = vec4(color.rgb, color.a*0.9*elecBrightness);
	}	
	else if (useTex == 1)
	{
		color = vec4(texture(braintex,TexCoord).rgb, headBrightness);
	}
	else
	{
		vec4 tmp2 = texture(activitytex,TexCoord);

		if (col == 0) // red
			color = vec4(tmp2.r*0.8, 0, tmp2.r*0.2, tmp2.a*mag*0.95);
		else if (col == 1) // blue
			color = vec4(tmp2.r*0.2, 0, tmp2.r*0.8, tmp2.a*mag*0.95);
		else // off
			color = vec4(tmp2.rgb,0.0);
	}
}