#version 410

layout (location = 0) in vec2 vertex;
uniform vec2 offset;

void main(void){
   vec2 resetpos = vertex + offset;

   resetpos.y = 1.0 - resetpos.y;

   gl_Position = vec4(resetpos, 0.0, 1.0);
}
