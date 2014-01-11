
#version 410


uniform sampler2DArray antennas;

out vec4 FragColor;

vec2 resolution = vec2(640.0, 480.0);


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


void main(void)
{	
	vec2 uv = gl_FragCoord.xy / resolution.xy;
	


	float totalPower = 0.0;


	for (int i = 0; i < 4; ++i)
	{
		float NDCDB = texture(antennas, vec3(uv, i)).r;
		totalPower += dBtoLin((NDCDB * 200.0) - 100.0);

	}
	
	float totalPowerDB = LintodB(totalPower);
	float totalPowerDBNDC = (totalPowerDB + 100.0) / 200.0;

	if (totalPowerDBNDC >= 0.5)
	{
		FragColor = vec4(totalPowerDBNDC, 0.0, 0.0, 1.0);
	}
	else
	{
		FragColor = vec4(0.0, totalPowerDBNDC, 0.0, 1.0);
	}

}
