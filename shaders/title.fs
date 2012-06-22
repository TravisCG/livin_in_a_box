#version 410

layout (location = 0) out vec4 FragColor;
uniform sampler2D image;
uniform float time;
in vec2 texcoord;
const vec2 center = vec2(0.5);

void main(void){

   float x = texcoord.x - 0.5;
   float y = texcoord.y - 0.5;
   float a = atan(x,y);
   float i = (sqrt(x*x + y*y) / (sin(a*8.0) * 0.25 + 0.95)) * pow(time, 2.0);
   i = smoothstep(0.0, 1.0, i);
   FragColor = mix(vec4(0.0,0.0,0.0,1.0), texture(image,texcoord), i);
}
