
uniform vec2 resolution;
uniform sampler2D logo;



void main()
{
	if (gl_FragCoord.x >= resolution.x || gl_FragCoord.y >= resolution.y)
	{
		discard;
	}

	vec2 uv = gl_FragCoord.xy / resolution.xy;
	vec4 sample = texture2D(logo, uv);

	if (sample.a <= 0.25)
	{
		discard;
	}

	gl_FragColor = sample;
}
