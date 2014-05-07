#version 410

#define M_PI 3.1415926535897932384626433832795

// This is the uniform that gets passed in
uniform vec2 AntPosition;

out vec4 FragColor;

// I don't know if I need this
bool floatEquals(float lhs, float rhs)
{
	return (abs(lhs - rhs) <= 0.5);
}

// Remaps values in the range -pi, pi to range 0, 1
float angleToNDC(float theta)
{
	return (theta + M_PI) / (M_PI * 2.0);
}

void main()
{
	vec2 pos_to_point = gl_FragCoord.xy - AntPosition;
	float theta = atan(pos_to_point.y, pos_to_point.x);
	// the actual value stored is in units of M_PI
	float angle = angleToNDC(theta);

	FragColor = vec4(angle, angle, 0.0, 1.0);
}
