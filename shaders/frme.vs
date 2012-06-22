#version 410

layout (location = 0) in vec2 vertex;
uniform float size;
uniform vec2 offset;
uniform float angle;
uniform vec2 center;

void main(void){
   vec2 resetpos = vertex * size;

//   resetpos.y = (1.0 - resetpos.y);
//   resetpos.x = (0.5 + resetpos.x);

   float ori = atan(resetpos.y - center.y, resetpos.x - center.x);
   float d = distance(resetpos, center);
   vec2 rot = vec2(d * cos(ori + angle), d * sin(ori + angle));
   resetpos = rot;

   gl_Position = vec4(resetpos + offset, -1.0, 1.0);
}
