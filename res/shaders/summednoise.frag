
#version 410

#define M_PI 3.1415926535897932384626433832795


uniform sampler2DArray angles;              // test 22
uniform sampler2DArray antennas;            // loss_array


uniform vec2 rcvr;                          // test 23

uniform sampler1DArray s_gain_patterns;       // test 17


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

// bool floatEquals(float lhs, float rhs)          // test 17
// {
// 	return (abs(lhs - rhs) <= 0.5);             // test 17
// }

// Remaps values in the range -pi, pi to range 0, 1
// float angleToNDC(float theta)                   // new
// {
// 	return (theta + M_PI) / (M_PI * 2.0);       // new
// }

// Takes an angle in 0-1 Units and returns the gain sampled at that point
// float gain(float angle, float layer)     // test 17
// {   // s_gain_patterns is -100..100 representing dB of gain
// 	return texture(s_gain_patterns, vec2(angle, layer)).x;  // test 17
//}


void main(void)
{	
	vec2 uv = gl_FragCoord.xy / resolution.xy;

	float totalPower = 0.0;
    int i = 0;
//	for (int i = 0; i < 1; ++i)
//	{
        // angles values range from 0..1, representing -Pi..Pi
        float offsetAngle = texture(angles, vec3(uv, i)).r;
        
    	// antennas values range from 0..1, representing -100..100
        float NDCDB = texture(antennas, vec3(uv, i)).r;
    
        float testRcvr = rcvr.x/rcvr.y;
        
        float testGain = texture(s_gain_patterns, vec2(rcvr.x,0.0)).x;
        
//        NDCDB += offsetAngle/2000000.0 + testRcvr/2000000.0 + testGain/2000000.0;
        
        // rcvr.x is the pointing angle of the receiver
        // it is stored in units of Pi, i.e. 0 = -Pi, 1 = Pi
//        offsetAngle = offsetAngle - rcvr.x;
//        if (offsetAngle < 0)
//            offsetAngle = 1 + offsetAngle;
//        else if (offsetAngle > 1)
//            offsetAngle = 1 - offsetAngle;
        
        // gain_patterns stores antenna gain as a 32 bit values from -100 to 100
        // rcvrPt.y is the gain of the antenna
//        float rcvrGain = rcvr.y;       // test 20
//        float rcvrGain = rcvr.y;
//        NDCDB +=rcvrGain;                                         // test 20
//        NDCDB = rcvrGain/4096 + NDCDB + offsetAngle/4096 + rcvrPt.x/4096;      // test 21
//        totalPower += dBtoLin(((NDCDB )* 200.0) - 100.0);
//        totalPower = offsetAngle;      // test 2
//  }
    
//    float totalPowerDB = LintodB(totalPower);
//	float totalPowerDBNDC = (totalPowerDB + 100.0) / 200.0;
//    float totalPowerDBNDC = totalPower;
    
//	if (totalPowerDBNDC >= 0.5)
//	{
//		FragColor = vec4(totalPowerDBNDC, 0.0, 0.0, 1.0);
//	}
//	else if ((totalPowerDBNDC >= 0.45))
//	{
//		FragColor = vec4(0.0, totalPowerDBNDC, 0.0, 1.0);
//	}
//    else
//	{
//		FragColor = vec4(0.0, 0.0, totalPowerDBNDC, 1.0);
//	}
    FragColor = vec4(offsetAngle, 0.0, 0.0, 1.0);
}
