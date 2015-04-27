#version 120

uniform float time;
uniform vec2 resolution;
uniform vec3 color;
uniform vec3 colorFreq;
uniform vec3 coeff1;
uniform vec3 coeff2;


void main( void ) {

	vec2 position = gl_FragCoord.xy / resolution.xy;

	float z = 0.0;
	z += cos( sin( position.x * cos( time / 15.0 ) * coeff1.x ) + 15);
	z += sin( cos( position.y * cos( time / 15.0 ) * coeff1.y ) + 25);
	z += cos( sin( position.y * sin( time / 10.0 ) * coeff1.z ) + 35);
	z += sin( cos( position.x * sin( time / 25.0 ) * coeff2.x ) + 45);
	z += sin( sin( position.x * cos( time / 50.0 ) * coeff2.y ) + 55);
	z += cos( sin( position.y * sin( time / 35.0 ) * coeff2.z ) + 65);

	z *= sin( time / 10.0 ) * 0.5;

	float r = sin(10.0 + z + time / colorFreq.x ) * color.x;
	float g = sin(20.0 + z + time / colorFreq.y ) * color.y;
	float b = sin(30.0 + z + time / colorFreq.z ) * color.z;
	
	gl_FragColor = vec4(r, g, b, 1.0 );


}