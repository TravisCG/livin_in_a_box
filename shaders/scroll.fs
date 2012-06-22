#version 410

layout (location = 0) out vec4 FragColor;
uniform sampler2D image;
uniform float wtime;
in vec2 texcoord;
in float dist;
const vec4 black = vec4(0.0, 0.0, 0.0, 1.0);

void main(void){
   vec2 t = texcoord;
   t.x += wtime * 0.7;
   FragColor = mix(texture(image,t), black, 1.0 - dist);
}
