#define M_PI 3.1415926535897932384626433832795

uniform sampler1D oned;

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

vec4 power(mat3 orientation)
{

	vec2 centerScreen = (orientation * vec3(0.0, 0.0, 1.0)).xy;
	vec2 centerToPoint = gl_FragCoord.xy - centerScreen;
	vec2 centerToPointRot = normalize((orientation * vec3(centerToPoint, 0.0)).xy);

	float theta = atan(centerToPointRot.y, centerToPointRot.x);
	
	//gl_FragColor = vec4(angleToNDC(theta), 0.0, 0.0, 1.0);

	float gain = gain(theta);
	float txpower = 25.0;
	float alpha = 2.0;

	// gain between 0, 70
	// 10.0 is from the dB conversion
	// 50.0 is the length of each pixel (50m) 640x480 is 32km by 24km
	// using log10

	float powerDB =
		txpower +
		(gain * 70.0);// -
		(10.0 * alpha * log(50.0*length(centerToPoint)) / log(10.0));
	// assume powerDB falls in the range -100, 100



	float powerNDC = (powerDB + 100.0) / 200.0;
	return vec4(powerNDC, powerNDC, powerNDC, 1.0);
}

void main()
{

	
	
	gl_FragColor = power(orientation1);
	gl_FragColor += power(orientation2);
	gl_FragColor += power(orientation1 * orientation2);

	/* if (gl_FragColor.x <= 0.5) */
	/* { */
	/* 	gl_FragColor = vec4(0.0, 0.0, 0.0, 1.0); */
	/* } */

	/* // Gain pattern overlay */
	/* vec2 resolution = vec2(640.0, 480.0); */
	/* vec2 uv = gl_FragCoord.xy / resolution.xy; */

  /* //gl_FragColor = texture2D(nothing, uv); */

	/* if (uv.y <= texture1D(oned, uv.x).x) */
	/* { */
	/* 	gl_FragColor += vec4(0.2, 0.2, 0.2, 1.0); */
	/* } */


	
}
