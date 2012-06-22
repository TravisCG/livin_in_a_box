#version 410

in vec3 vertex;
uniform mat4 pmatrix;
uniform mat4 mmatrix;
out float depth;

void main(void){
   depth = normalize(vertex.z);
   gl_Position = pmatrix * mmatrix * vec4(vertex, 1.0);
}
