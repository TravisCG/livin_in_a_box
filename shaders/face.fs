#version 410

in float depth;
out vec4 gl_FragColor;

void main(void){
   gl_FragColor = vec4(depth);
}
