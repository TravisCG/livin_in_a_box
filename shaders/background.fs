#version 410

layout (location = 0) out vec4 FragColor;
uniform sampler2D image;
in vec2 texcoord;

void main(void){
   FragColor = texture(image,texcoord);
}
