#version 120

uniform vec2 resolution;
uniform sampler2DRect texture;
uniform float time;
uniform float row;
uniform float col;
uniform vec2 speed;

void main(void) {
	vec2 st = gl_FragCoord.st;
	
	float rx = 2.0 * abs(0.5 - mod(row + speed.y * time, 1.0));
	float ry = 2.0 * abs(0.5 - mod(col + speed.x * time, 1.0));
	
	vec2 p1 = vec2(st.x, rx * resolution.y);
	vec2 p2 = vec2(ry * resolution.x, st.y);
	
	float r = texture2DRect(texture, p1).r * texture2DRect(texture, p2).r;
	float g = texture2DRect(texture, p1).g * texture2DRect(texture, p2).g;
	float b = texture2DRect(texture, p1).b * texture2DRect(texture, p2).b;

  	gl_FragColor = vec4(r, g, b, 1.0);
}

