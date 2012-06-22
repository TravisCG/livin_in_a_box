#version 410

layout (location = 0) in vec2 vertex;
uniform vec2 offset;
uniform float time;
out vec2 tc;

void main(void){
   float scroll = vertex.x + offset.x - time;
   tc = ceil(vertex / 3.0);
   gl_Position = vec4(scroll, vertex.y + offset.y, -1.0, 1.0);
}
