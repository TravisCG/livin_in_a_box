#version 410

layout (triangles) in;
layout (triangle_strip, max_vertices = 99) out;

in vec2 tcoord2[];
in vec3 normal2[];

uniform float time;
uniform mat4 pmatrix;

out vec2 tcoord3;
out vec3 normal3;

const float PI = 3.141592;

void main(){
   int i,j;
   mat4 pos = mat4(1.0);
   int max = int(time) < 33 ? int(time) : 33;
   for(j = 0; j < max; j++){
      float f = (2.0 * PI) * (j / 12.0);
      pos[3][0] = sin(time + f) * (4.0 * sin(time));
      pos[3][1] = cos(time + f) * (4.0 * sin(time));
      pos[3][2] = -10.0 - (j * 1.5);

      for(i = 0; i < gl_in.length(); i++){
         gl_Position = pmatrix * pos * gl_in[i].gl_Position;
         tcoord3 = tcoord2[i];
         normal3 = normal2[i];
         EmitVertex();
      }

      EndPrimitive();
   }

}
