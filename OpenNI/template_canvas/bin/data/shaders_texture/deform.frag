#version 120

uniform sampler2DRect texture;
uniform vec2 resolution;
uniform float time;
uniform float speed;
uniform vec2 mouse;
uniform float turns;

void main(void) {
  vec2 p = -1.0 + 2.0 * gl_FragCoord.st / resolution.xy;
  vec2 m = -1.0 + 2.0 * mouse;

  float a1 = abs(atan(p.y - m.y, p.x - m.x));
  float r1 = sqrt(dot(p - m, p - m));
  float a2 = abs(atan(p.y + m.y, p.x + m.x));
  float r2 = sqrt(dot(p + m, p + m));

  vec2 uv;
  uv.x = time*speed + (r1 - r2) * 0.25;
  uv.y = sin(turns * (a1 - a2));
  float w = r1 * r2 * 0.5;

  vec3 col = texture2DRect(texture, resolution.xy * mod(uv,1.0)).xyz;
  gl_FragColor = vec4(col, 1.0);
}