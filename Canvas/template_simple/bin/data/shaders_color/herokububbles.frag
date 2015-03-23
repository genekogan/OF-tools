#version 120

uniform float time;
uniform vec2 mous;
uniform vec2 resolution;
uniform float fRadius;
uniform float numBubbles;
uniform vec3 color1, color2;
uniform float dir;
uniform vec2 speed;

void main(void)
{
	vec2 uv = -1.0 + 2.0*gl_FragCoord.xy / resolution.xy;
	uv.x *=  resolution.x / resolution.y;
	
	vec4 color = vec4(0.0, 0.0, 0.0, 0.0);

    // bubbles
	for( float i=0.0; i<numBubbles; i++ )
	{
        // bubble seeds
		float pha = sin(i*5.13+1.0)*0.5 + 0.5;
		float siz = pow( sin(i*1.74+5.0)*0.5 + 0.5, 4.0 );
		float pox = sin(i*3.55+4.1) * resolution.x / resolution.y;
		
        	// buble size, position and color
		vec2  pos = vec2( pox+sin(time/dir+pha+siz), -1.0-fRadius + (20.0+30.0*fRadius)
						 *mod(pha+speed.x*(time/5.)*(speed.y+0.8*siz),1.0)) * vec2(1.0, 1.0);
		float dis = length( uv - pos );

		
        // render
		if(dis < fRadius){
			float a = (1.- smoothstep( fRadius*(0.65+0.20*sin(pox*time)), fRadius, dis )) * (1.0 - cos(pox*time));
			vec3  col = mix( color1, color2, 0.5+0.5*sin(i*sin(time*pox*0.03)+1.9));
			color.xyz += (col.xyz * a);	
			
			color.a += a;
		}
	}

	gl_FragColor = color;
}