#version 410

in vec3 vertex;
in vec3 normal;
in vec2 tcoord;

out vec3 normal2;
out vec2 tcoord2;

void main(){
   normal2 = normal;
   tcoord2 = tcoord;
   gl_Position = vec4(vertex, 1.0);
}
