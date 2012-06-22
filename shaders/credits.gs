#version 410

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;
out vec2 texcoord;

void main(){
  int i;

  for(i = 0; i < 3; ++i){
     gl_Position.xy = gl_in[i].gl_Position.xy;
     texcoord = gl_in[i].gl_Position.xy / 2.0 + 0.5;

     EmitVertex();
  }

  EndPrimitive();
}
