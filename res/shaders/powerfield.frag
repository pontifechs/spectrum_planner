#define M_PI 3.1415926535897932384626433832795

uniform vec2 resolution;

uniform sampler1D oned;
uniform sampler2D global_alpha;

uniform mat3 orientation1;
uniform mat3 orientation2;

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
	return texture1D(oned, angleToNDC(angle)).x;
}

float alpha(vec2 screenSpace)
{
	float alpha = (texture2D(global_alpha, screenSpace / resolution).x * 255.0 / 10.0) + 2.0;

	if (!(2.0 <= alpha && alpha <= 27.5))
	{
		return -10000.0;
	}

	return alpha;
}

float log10(float val)
{
	return log2(val) / log2(10.0);
}

float dBtoLin(float dB)
{
	return pow(10.0, dB / 10.0);
}

float LintodB(float lin)
{
	return 10.0 * log10(lin);
}


float powerDB(mat3 orientation)
{

	vec2 centerScreen = (orientation * vec3(0.0, 0.0, 1.0)).xy;
	vec2 centerToPoint = gl_FragCoord.xy - centerScreen;
	vec2 centerToPointRot = normalize((orientation * vec3(centerToPoint, 0.0)).xy);

	float theta = atan(centerToPointRot.y, centerToPointRot.x);
	float gain = gain(theta);
	float txpower = 0.0;
	float alpha = (texture2D(global_alpha, centerScreen / resolution).x * 255.0 / 10.0) + 2.0;

	// gain between 0, 70 (set by gain pattern generator, only assumed here.)
	// 10.0 is from the dB conversion
	// 50.0 is the length of each pixel (50m) 640x480 is 32km by 24km
	// using log10

	float raySegmentLength = length(50.0 * centerToPoint / 25.0);

	float powerDB = txpower + gain - (10.0 * alpha * log10(raySegmentLength));

	// March a ray through the global_alpha map to approximate non-constant alpha attenuation.
	for (int i = 1; i < 25; ++i)
	{
		// The intermediate position (current ray position)is defined as follows:
		// centerScreen + t * centerToPoint
		// where t goes from 0.0 to 1.0 in constant intervals
		vec2 ray = centerScreen + (float(i) / 25.0 * centerToPoint);
		float alpha = (texture2D(global_alpha, ray/resolution).x * 255.0/ 10.0)+ 2.0;

		powerDB -= 10.0 * alpha * log10(float(i + 1) / float(i));
	}
 	
	return powerDB;
}

void main()
{

	float power1DB = powerDB(orientation1);
	float power2DB = powerDB(orientation2);
	float power = dBtoLin(power1DB) + dBtoLin(power2DB);
	float sumPowerDB = LintodB(power);
	float sumPowerDBNDC = (sumPowerDB + 100.0) / 200.0;

	// Make positive dB red, negative dB green.
	if (sumPowerDBNDC >= 0.5)
	{
		gl_FragColor = vec4(sumPowerDBNDC, 0.0, 0.0, 1.0);
	}
	else
	{
		gl_FragColor = vec4(0.0, sumPowerDBNDC, 0.0, 1.0);
	}



	// global alpha overlay
	/* vec2 uv = gl_FragCoord.xy / resolution.xy; */
	/* gl_FragColor += 0.3 * texture2D(global_alpha, uv); */



/* 	// Gain pattern overlay */
/* 	vec2 uv = gl_FragCoord.xy / resolution.xy; */

/* //  gl_FragColor = texture2D(global_alpha, uv); */

/* 	if (uv.y * 70.0 <= texture1D(oned, uv.x).x) */
/* 	{ */
/* 		gl_FragColor += vec4(0.2, 0.2, 0.2, 1.0); */
/* 	} */


	
}
