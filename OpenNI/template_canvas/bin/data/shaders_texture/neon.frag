#version 120

uniform sampler2DRect texture;
uniform float brt;
uniform float rad;

//const vec2 texOffset = vec2(1.0, 1.0);

void main(void) {
  float i = 0.0;
  float j = 0.0;
  vec4 sum = vec4(0.0);
  
  for( i=-rad;i<rad;i+=1.0) {
    for( j=-rad;j<rad;j+=1.0) {
        sum += texture2DRect( texture, gl_FragCoord.st + vec2(j,i))*brt;
    }
  }

  gl_FragColor = sum*sum+ vec4(texture2DRect( texture, gl_FragCoord.st).rgb, 1.0);
}
