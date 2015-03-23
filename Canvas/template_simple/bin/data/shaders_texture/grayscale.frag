#version 120

uniform sampler2DRect texture;

void main(void)
{
	vec3 col = texture2DRect(texture, gl_FragCoord.st).rgb;
	float bright = 0.33333 * (col.r + col.g + col.b);
    gl_FragColor = vec4(vec3(bright), 1.0);
}
