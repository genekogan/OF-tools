#version 120

uniform sampler2DRect texture;

uniform vec3 biasx, biasy;
uniform vec3 multx, multy;

void main(void) {	
    vec2 st = gl_FragCoord.st;
    //vec2 st = pos.xy;
	vec2 pr = vec2(multx.x, multy.x) * st + vec2(biasx.x, biasy.x);
	vec2 pg = vec2(multx.y, multy.y) * st + vec2(biasx.y, biasy.y);
	vec2 pb = vec2(multx.z, multy.z) * st + vec2(biasx.z, biasy.z);

	float r = texture2DRect(texture, pr).r;
	float g = texture2DRect(texture, pg).g;
	float b = texture2DRect(texture, pb).b;
  	gl_FragColor = vec4(r, g, b, 1.0);
}

