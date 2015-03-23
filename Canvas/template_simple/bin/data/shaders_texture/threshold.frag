#version 120

uniform sampler2DRect texture;
uniform float threshold;

void main(void)
{
	vec3 col = texture2DRect(texture, gl_FragCoord.st).rgb;
	float bright = 0.33333 * (col.r + col.g + col.b);
	float b = mix(0.0, 1.0, step(threshold, bright));
    gl_FragColor = vec4(vec3(b), 1.0);
}
