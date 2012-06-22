#version 410

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;
out vec2 texcoord;
out float dist;
uniform float wtime;
uniform float distfactor;

void main(){
  int i;

  for(i = 0; i < 3; ++i){
     vec2 old = gl_in[i].gl_Position.xy;

     if(old.x != 1 && old.x != -1){
        old.x = old.x + sin(wtime + gl_in[i].gl_Position.x) / 25.0;
     }

     if(old.y != 1 && old.y != -1){
        old.y = old.y + cos(wtime + gl_in[i].gl_Position.y) / 10.0;
     }

     gl_Position.xy = old;
     dist = 1.0 - ( abs(sin(wtime / 3.0) / 1.0)) * distfactor;
     texcoord = gl_in[i].gl_Position.xy / (2.0 * dist) + 0.5;

     EmitVertex();
  }

  EndPrimitive();
}
