#version 410

uniform sampler2DArray viewA;

out vec4 FragColor;

void main(void)
{
    vec2  uv = gl_FragCoord.xy / vec2(640.0,480.0);
    float rVal = texture(viewA,vec3(uv,0)).x;

    if (rVal >= 0.5)
	{
		FragColor = vec4(rVal, rVal, rVal, 1.0);
	}
	else if ((rVal >= 0.45))
	{
		FragColor = vec4(rVal, rVal, 0.0, 1.0);
	}
    else
	{
		FragColor = vec4(0.0, 0.0, rVal, 1.0);
    }
    
}