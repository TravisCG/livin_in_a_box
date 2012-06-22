#version 410

precision highp float;
in vec3 color;
out vec4 gl_FragColor;

void main(void){
   gl_FragColor = vec4(color, 1.0);
}
