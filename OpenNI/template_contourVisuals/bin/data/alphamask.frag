#version 120

uniform sampler2DRect tex0;
uniform sampler2DRect maskTex;

void main (void) {
	//vec2 pos = gl_TexCoord[0].st;
	vec2 pos = gl_FragCoord.st;
	vec3 src = texture2DRect(tex0, pos).rgb;
	float mask = texture2DRect(maskTex, pos).r;
	gl_FragColor = vec4( src , mask);
}