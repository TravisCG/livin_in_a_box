#version 410

in vec3 vertex;
in vec3 normal;

uniform mat4 pmatrix;
uniform mat4 camera;

out vec3 color;

void main(void){
   gl_Position = pmatrix * camera * vec4(vertex, 1.0);
   float lum = dot(normal, vec3(-1.0));
   color = vec3(lum);
}
