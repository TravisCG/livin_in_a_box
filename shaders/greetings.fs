#version 410

layout (location = 0) out vec4 FragColor;
in vec2 tc;
uniform sampler2D img;

void main(void){
   FragColor = texture(img, tc);
}
