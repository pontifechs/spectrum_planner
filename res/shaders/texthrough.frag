
#version 410

#define M_PI 3.1415926535897932384626433832795

out vec4 FragColor;

uniform sampler2D global_alpha;

vec2 resolution = vec2(640.0, 480.0);

void main(void)
{
	vec2 uv = gl_FragCoord.xy / resolution.xy;
	FragColor = texture(global_alpha, uv);
}
