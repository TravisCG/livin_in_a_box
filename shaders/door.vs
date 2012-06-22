#version 410

layout (location = 0) in vec2 vertex;

void main(void){
   float scale = 2.0;
   vec2 resetpos = vertex * scale;

   resetpos.y = 1.0 - resetpos.y;
   resetpos.x = resetpos.x - 1.05;

   gl_Position = vec4(resetpos, -1.0, 1.0);
}
