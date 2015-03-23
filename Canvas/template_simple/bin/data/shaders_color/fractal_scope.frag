#version 120

uniform vec2 resolution;
uniform float time;
uniform float angle;
uniform float zoom;
uniform float iterations;

/*
 * inspired by http://www.fractalforums.com/new-theories-and-research/very-simple-formula-for-fractal-patterns/
 * a slight(?) different 
 * public domain
 */


void main( void ) {
	vec2 v = (gl_FragCoord.xy - resolution/2.0) / min(resolution.y,resolution.x) * 20.0;

	float rsum = 0.0;
	float pi2 = 3.1415926535 * 2.0;
	float C = cos(angle * pi2);
	float S = sin(angle * pi2);
	vec2 shift = vec2( 0.4, 1.0 );
	float zoom2 = (zoom*1.0 + 1.0);
	
	for ( int i = 0; i < iterations; i++ ){
		float rr = v.x*v.x+v.y*v.y;
		if ( rr > 1.0 ){
			rr = 1.0/rr;
			v.x = v.x * rr;
			v.y = v.y * rr;
		}
		rsum *= 0.99;
		rsum += rr;
		
		v = vec2( C*v.x-S*v.y, S*v.x+C*v.y ) * zoom2 + shift;
	}
	
	float col = rsum * 0.5;


	gl_FragColor = vec4( cos(col*1.0), cos(col*2.0), cos(col*4.0), 1.0 );

}