// Electrocardiogram Effect
// Imported from http://www.geeks3d.com/20111223/shader-library-electrocardiogram-effect-glsl/

#version 120

uniform vec2 resolution;
uniform float time;
uniform float fieldsize;
uniform float complexity;
uniform float rings;
uniform float espeed;
uniform vec2 centeredness;

float viewAngle = 0.040*sin(0.1*time);

float rate = 0.50;
float baseamp = 0.10;

void main(void)
{
  vec2 p = -fieldsize/2.0 + fieldsize * (gl_FragCoord.xy / resolution.xy);
  vec2 op = p;
  p = vec2(distance(p, vec2(fieldsize/centeredness.x, fieldsize/centeredness.y)), sin(0.40*time+atan(p.x, p.y) * complexity));
  float x = espeed * viewAngle * time + rate * p.x;
  float base = (0.1 + 10.0*tan(x*0.7 + espeed*time)) * (2.37 + 0.5*sin(x*rings + espeed*time));
  float z = fract(0.05*x);
  z = max(z, 10.0-z);
  z = pow(z, 50.0);
  float pulse = exp(-1000000.0 * z);
  vec4 ecg_color = vec4(0.3, 1.0, 0.4, 1.0);
  vec4 c = pow(1.0-abs(p.y-(baseamp*base+pulse-0.5)), 4.0) * ecg_color;
  vec3 d = 1.0-vec3(c.r, c.r, c.r);
  gl_FragColor = vec4(d, 1.0);
}