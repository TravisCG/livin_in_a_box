#version 410

layout (location=0) in vec3 vertex; 
out vec2 texcoord;
uniform mat4 pmatrix;

void main(void){
   gl_Position = pmatrix * vec4(vertex, 1.0);
   texcoord = vertex.xy / 200 + 0.5;
}
