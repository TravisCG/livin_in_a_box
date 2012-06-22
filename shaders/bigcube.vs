#version 410

layout (location = 0) in vec3 vertex;

uniform mat4 pmatrix;
uniform mat4 camera;

out vec3 color;

void main(void){
   color = vec3(abs(vertex.z));
   gl_Position = pmatrix * camera * vec4(vertex, 1.0);
}
