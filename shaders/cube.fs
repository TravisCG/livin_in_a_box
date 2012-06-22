#version 410

layout (location = 0) out vec4 FragColor;
in vec3 normal3;
in vec2 tcoord3;

uniform sampler2D img;
uniform float time;

const vec3 lightpos = normalize(vec3(0.0, 1.0, 1.0));

void main(){
   vec4 tcolor = texture(img, tcoord3);
   vec3 bcolor = vec3(0.0, 1.0, 0.0);
   vec3 acolor = vec3(1.0, 0.0, 0.0);
   float lambert = max(dot(normal3, lightpos), 0.5);
   vec3 final = mix(acolor, bcolor, tcolor.r) * lambert;
   FragColor = vec4(final,1.0);
}
