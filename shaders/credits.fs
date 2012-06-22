#version 410
/*
Code base on:
http://www.geeks3d.com/20110428/shader-library-swirl-post-processing-filter-in-glsl/
*/
layout (location = 0) out vec4 FragColor;
uniform sampler2D image;
uniform float time;
in vec2 texcoord;
const vec4 bgcolor = vec4(0.1019, 0.1450, 0.0, 1.0);
const vec2 center = vec2(0.5, 0.5);

void main(void){
   float radius = 0.75;
   float angle = time * 1.5;
   vec2 newtc = texcoord - center;
   float dist = length(newtc);
   if(dist < radius){
      float percent = (radius - dist) / radius;
      float theta = percent * percent * angle * 8.0;
      float s = sin(theta);
      float c = cos(theta);
      newtc = vec2(dot(newtc,vec2(c,-s)), dot(newtc, vec2(s,c)));
   }
   newtc += center;
   vec4 texcolor = texture(image, newtc);
   FragColor = mix(bgcolor,texcolor,texcolor.a);
}
