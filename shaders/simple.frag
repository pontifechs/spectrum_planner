
uniform sampler2D tex;

void main()
{

  //gl_FragColor = vec4(0.5,0.0,0.5,1.0);
	vec2 uv = gl_FragCoord.xy / vec2(640,480);
	gl_FragColor = texture2D(tex, uv);
}
