
#version 410

#define M_PI 3.1415926535897932384626433832795

uniform sampler1DArray gain_patterns;
uniform float gainPatternLayer;

out vec4 FragColor;

vec2 resolution = vec2(640.0, 480.0);

// Remaps values in the range -pi, pi to range 0, 1
float angleToNDC(float theta)
{
	return (theta + M_PI) / (M_PI * 2.0);
}

// Calculates the opposite angle (180 degrees offset)
float oppositeAngle(float theta)
{
	if (theta >= 0)
	{
		return theta + (M_PI / 2);
	}
	else
	{
		return theta - (M_PI / 2);
	}
}

// Takes an angle in radians and returns the gain sampled at that point
float gain(float angle)
{
	return texture(gain_patterns, vec2(angleToNDC(angle), gainPatternLayer)).x;
}



void main(void)
{
	vec2 pos_screen = vec2(0.5, 0.5) * resolution ;
	vec2 pos_to_point = gl_FragCoord.xy - pos_screen;
	
	float theta = atan(pos_to_point.y, pos_to_point.x);
	float gain = gain(theta);

	// Check the main angle
	if (length(pos_to_point) <= 8*(gain + 18))
	{
		FragColor = vec4(0.5, 0.5, 0.5, 1.0);
	}
	else
	{
		FragColor = vec4(1.0, 1.0, 1.0, 1.0);
	}

	// Check the opposite angle (180 around)


}
