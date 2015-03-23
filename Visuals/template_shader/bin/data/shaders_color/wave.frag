#version 120

uniform float time;
uniform vec2 resolution;
uniform vec2 r, t;
uniform float bright;
uniform vec2 mode;
uniform float redshift;
uniform float multiplicity;
uniform float fieldsize;



void main( void ) {

	vec2 p = fieldsize * (( gl_FragCoord.xy / resolution.xy ) - 0.5);
	
	float x = mix(p.x, p.y/p.x, mode.x);
	float y = mix(p.x*p.y, p.y*p.x, 1.0-mode.x);
	float z = mix(p.x/p.y, p.x*p.y*p.x / 10.0, mode.y);
	
	float sx = fieldsize * multiplicity * sin( r.x * x - t.x * time);
	sx = sx * cos( r.y * x + t.y * time);
	
	float dy = bright / ( 30. * abs(z - sx));
	
	float d = 1.0-smoothstep(fieldsize*0.5 - max(abs(p.x), abs(p.y)), 0.0, 0.012);
	
	vec3 col2 = vec3((p.x + redshift)*p.x*p.y * dy, 2.9 * dy, 1.3*dy-1.35);
	vec3 col = vec3(max(col2.r, d), max(col2.g, d), max(col2.b, 0.0));
	gl_FragColor = vec4( col, 1.0 );

}