#version 410

#define M_PI 3.1415926535897932384626433832795

out vec4 FragColor;

uniform sampler2DArray antennas;
uniform float antennaLayer;

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

    
	// antennas values range from 0..1, representing -100..100
	float NDCDB = texture(antennas, vec3(uv, antennaLayer)).r;
    
	totalPower += dBtoLin(((NDCDB )* 200.0) - 100.0);

	float totalPowerDB = LintodB(totalPower);
	float totalPowerDBNDC = (totalPowerDB + 100.0) / 200.0;
    
	FragColor = vec4(totalPowerDBNDC,
									 totalPowerDBNDC,
									 totalPowerDBNDC,
									 1.0);

}
