#version 410

out vec4 gl_FragColor;

void main(void){
   vec4 acolor = vec4(0.9058, 0.9137, 0.5921, 1.0);
   vec4 bcolor = vec4(0.9686, 0.9764, 0.9882, 1.0);
   vec4 mixcolor = mix(acolor, bcolor, sin(gl_FragCoord.x)/5.0);
   gl_FragColor = mixcolor;
}
