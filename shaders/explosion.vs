#version 410
layout (location = 0) in vec3 vertex;

uniform float time;
const vec3 center = vec3(0.0);

void main(void){
   vec3 interpolate = mix(center, vertex, time);
   gl_Position = vec4(interpolate, 1.0);
}
