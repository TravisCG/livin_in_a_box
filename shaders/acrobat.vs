#version 410

layout (location = 0) in vec2 vertex;
uniform vec2 avg;
uniform vec2 offset;

void main(void){
   vec2 resetpos = vertex;

   resetpos.y = 1.0 - resetpos.y;
   resetpos.x = resetpos.x - avg.x;
   resetpos = resetpos * 3.5;
   gl_Position = vec4(resetpos + offset, -1.0, 1.0);
}
