#version 410

layout (location = 0) in vec2 vertex;

void main(void){
   vec2 repos = 1.0 - vertex * 2.75;
   repos.x += 1.0;
   repos.y += 0.3;
   gl_Position = vec4(repos, -1.0, 1.0);
}
