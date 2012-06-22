#version 410

layout (location = 0) in vec2 vertex;
uniform float avg;
uniform vec2 offset;

void main(void){
   vec2 resetpos = vertex;

   resetpos.y = 1.0 - resetpos.y;
   resetpos.x = 0.5 - (avg - resetpos.x);

   gl_Position = vec4(resetpos + offset, -1.0, 1.0);
}
