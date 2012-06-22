#version 410
layout (location = 0) in vec2 vertex;
uniform vec2 voffset;

void main(void){
   gl_Position = vec4(vertex + voffset, 0.0, 1.0);
}
