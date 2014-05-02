#version 410

#define M_PI 3.1415926535897932384626433832795


uniform sampler2DArray angles;              // angle offset array
uniform sampler2DArray antennas;            // loss_array
uniform sampler2DArray viewA;               // view B at layer 0

uniform vec2 rcvr;
// rcvr.x is thr azimuth angle
// rcvr.y is the gain pattern index

uniform sampler1DArray gain_patterns;

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

// Takes an angle in 0-1 Units and returns the gain sampled at that point
float gain(float angle, float layer)
{   // gain_patterns is -100..100 representing dB of gain
 	return texture(gain_patterns, vec2(angle, layer)).x;
}


void main(void)
{	
	vec2 uv = gl_FragCoord.xy / resolution.xy;

	float totalPower = 0.0;

	for (int i = 0; i < 8; ++i)
{
        // angles values range from 0..1, representing -Pi..Pi
        float offsetAngle = texture(angles, vec3(uv, i)).r;
        offsetAngle = offsetAngle - 0.5;
        // rcvr.x is the pointing angle of the receiver
        // it is stored in units of 2*Pi, i.e. 0 = -Pi, 1 = Pi
        offsetAngle = offsetAngle - rcvr.x;
        if (offsetAngle < 0)
            offsetAngle = 1 + offsetAngle;
        else if (offsetAngle > 1)
            offsetAngle = 1 - offsetAngle;
    
    	// antennas values range from 0..1, representing -100..100
        float NDCDB = texture(antennas, vec3(uv, i)).r;
    
        float rcvrGain = ( gain(offsetAngle,rcvr.y)/200.0 );
    
        NDCDB += rcvrGain;
    
        totalPower += dBtoLin(((NDCDB )* 200.0) - 100.0);
}
    
    float totalPowerDB = LintodB(totalPower);
	float totalPowerDBNDC = (totalPowerDB + 100.0) / 200.0;
    float altView = texture(viewA, vec3(uv ,0)).x;
    // take the highest value
    float maxValue;
    if (altView > totalPowerDBNDC) {
        maxValue = altView;
    }
    else {
        maxValue = totalPowerDBNDC;
    }

    FragColor = vec4(maxValue, 0.0, 0.0, 1.0);
}
