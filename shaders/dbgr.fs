#version 410

layout (location = 0) out vec4 FragColor;
uniform sampler2D image;
in vec2 texcoord;

void main(void){
   FragColor = min(texture(image,texcoord), 0.5);
}
