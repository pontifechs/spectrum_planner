#version 410

#define M_PI 3.1415926535897932384626433832795

uniform vec2 resolution;

uniform sampler1DArray gain_patterns;
uniform sampler2D global_alpha;

struct Antenna
{
	mat3 orientation;
	float power;
	float gainPattern;
};

uniform Antenna antenna;

out vec4 FragColor;


bool floatEquals(float lhs, float rhs)
{
	return (abs(lhs - rhs) <= 0.5);
}

// Remaps values in the range -pi, pi to range 0, 1
float angleToNDC(float theta)
{
	return (theta + M_PI) / (M_PI * 2.0);
}

// Takes an angle in radians and returns the gain sampled at that point
float gain(float angle)
{
	return texture(gain_patterns, vec2(angleToNDC(angle), antenna.gainPattern)).x;
}

float alpha(vec2 screenSpace)
{
	float alphaSample = (texture(global_alpha, screenSpace / resolution).x * 255.0 / 10.0) + 2.0;

	if (!(2.0 <= alphaSample && alphaSample <= 27.5))
	{
		return -10000.0;
	}

	return alphaSample;
}

float log10(float val)
{
	return log2(val) / log2(10.0);
}

float powerDB(Antenna ant)
{

	vec2 pos_screen = (ant.orientation * vec3(0.0, 0.0, 1.0)).xy;
	vec2 pos_to_point = gl_FragCoord.xy - pos_screen;
	vec2 centerToPointRot = normalize((ant.orientation * vec3(pos_to_point, 0.0)).xy);

	float theta = atan(centerToPointRot.y, centerToPointRot.x);
	float gain = gain(theta);
	float txpower = 12.0;
	float alpha = (texture(global_alpha, pos_screen / resolution).x * 255.0 / 10.0) + 2.0;

	// gain between 0, 70 (set by gain pattern generator, only assumed here.)
	// 10.0 is from the dB conversion
	// 50.0 is the length of each pixel (50m) 640x480 is 32km by 24km
	// using log10

	float raySegmentLength = length(50.0 * pos_to_point / 25.0);

	float powerDB = ant.power + gain - (10.0 * alpha * log10(raySegmentLength));

	// March a ray through the global_alpha map to approximate non-constant alpha attenuation.
	for (int i = 1; i < 25; ++i)
	{
		// The intermediate position (current ray position)is defined as follows:
		// centerScreen + t * centerToPoint
		// where t goes from 0.0 to 1.0 in constant intervals
		vec2 ray = pos_screen + (float(i) / 25.0 * pos_to_point);
		float alpha = (texture(global_alpha, ray/resolution).x * 255.0/ 10.0)+ 2.0;

		powerDB -= 10.0 * alpha * log10(float(i + 1) / float(i));
	}
 	
	return powerDB;
}

void main()
{


	float power1DB = (powerDB(antenna) + 100.0) / 200.0;


	FragColor = vec4(power1DB, power1DB, power1DB, 1.0);	

	/* // Gain pattern overlay */
	/* vec2 uv = gl_FragCoord.xy / resolution.xy; */
	/* if (70.0 * uv.y <= texture(gain_patterns, vec2(uv.x, 1)).x) */
	/* { */
	/* 	FragColor += vec4(0.2, 0.2, 0.2, 1.0); */
	/* } */

	
}
