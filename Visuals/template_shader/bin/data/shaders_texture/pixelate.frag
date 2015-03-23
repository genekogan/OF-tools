#version 120

uniform sampler2DRect texture;
uniform vec2 pixelSize;

void main(void)
{
  	vec2 st = gl_FragCoord.st;

	st.x -= mod(st.x, pixelSize.x);
	st.y -= mod(st.y, pixelSize.y);
  
	vec3 texColor = texture2DRect(texture, st).rgb;
	gl_FragColor = vec4(texColor, 1.0);
}
