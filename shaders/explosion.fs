#version 410
in vec2 texcoord;
out vec4 gl_FragColor;
const float PI = 3.1416;

void main(void){
   vec2 f = texcoord * (PI * 2.0) - PI;
   float inter = cos(f.x) * cos(f.y);
   vec3 acolor = vec3( 0.317, 0.4, 0.305);
   vec3 bcolor = vec3( 0.294, 0.313, 0.325);
   gl_FragColor = vec4(mix(bcolor, acolor, inter),1.0);
}
