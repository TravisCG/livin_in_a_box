#version 410

layout (location = 0) in vec2 vertex;
uniform float growfactor;
uniform vec2 offset;

void main(void){
   vec2 resetpos;

   /* Take the picture to center */
   resetpos.x = (vertex.x - 0.3282) * growfactor / 20.0;
   resetpos.y = (0.5666 - vertex.y) * growfactor / 20.0;

   gl_Position = vec4(resetpos + offset, -1.0, 1.0);
}
