#version 120

uniform float time;
uniform vec2 resolution;
uniform vec2 colorMult;
uniform vec3 coeff;


void main( void ) {

	vec2 position = gl_FragCoord.xy / resolution.xy;

	float color = 0.0;
	color += sin( position.x * cos( time / 15.0 ) * 10.0 )  +  cos( position.y * cos( time / 15.0 ) * coeff.x );
	color += sin( position.y * sin( time / 10.0 ) * coeff.z )  +  cos( position.x * sin( time / 25.0 ) * coeff.y );
	color += sin( position.x * sin( time / 50.0 ) * coeff.x )  +  sin( position.y * sin( time / 35.0 ) * coeff.z );

	color *= sin( time / 10.0 ) * 0.5;

	float r = color;
	float g = color * colorMult.y;
	float b = sin( color + time / 2.0 ) * colorMult.x;
	
	gl_FragColor = vec4(r, g, b, 1.0 );

}