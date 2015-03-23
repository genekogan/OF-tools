#version 120

uniform sampler2DRect texture;
uniform vec2 resolution;
uniform float time;
uniform vec2 pixels;
uniform float margin;
uniform float rate;
uniform float dir;
uniform float gate;

void main(void) {
  	vec2 p = gl_FragCoord.xy / resolution.xy;

	float d = 0.5 - p.x;
	float d2 = mod(abs(d), pixels.x);
	p.x += dir * sign(d) * d2;
	p.x -= gate * mod(p.x, pixels.x);

	float e = 0.5 - p.y;
	float e2 = mod(abs(e), pixels.y);
	p.y += dir * sign(e) * e2;
	p.y -= gate * mod(p.y, pixels.y);	
	
	p.y = mod(p.y + margin*sin(rate * time * p.x + p.x), 1.0);
//	p.y = 1.0-p.y;

	float r = texture2DRect(texture, p*resolution.xy).r;
	float g = texture2DRect(texture, p*resolution.xy).g;
	float b = texture2DRect(texture, p*resolution.xy).b;
	
  	gl_FragColor = vec4(r, g, b, 1.0);
}

