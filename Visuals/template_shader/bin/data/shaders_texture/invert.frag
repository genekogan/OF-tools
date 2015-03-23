#version 120

uniform sampler2DRect texture;

void main(void)
{
  	vec2 p = gl_FragCoord.st;
	vec3 col = vec3(1.0) - texture2DRect(texture, p).rgb;
	gl_FragColor = vec4(col, 1.0);
}
