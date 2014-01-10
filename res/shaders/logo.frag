#version 410


uniform vec2 resolution;
uniform sampler2D logo;

out vec4 FragColor;

void main()
{
	if (gl_FragCoord.x >= resolution.x || gl_FragCoord.y >= resolution.y)
	{
		discard;
	}

	vec2 uv = gl_FragCoord.xy / resolution.xy;
	vec4 texSample = texture(logo, uv);

	if (texSample.a <= 0.25)
	{
		discard;
	}

	FragColor = texSample;
}
