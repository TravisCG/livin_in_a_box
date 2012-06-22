#version 410

layout (points) in;
layout (triangle_strip, max_vertices = 4) out;
out vec2 texcoord;
uniform float time;
const float pi2 = 1.57079632; /* pi / 2 */
const float dim = 10.0;

void createVertex(float angle){
  vec2 new = gl_in[0].gl_Position.xy + vec2(sin(angle), cos(angle)) / dim;
  gl_Position = vec4(new, -1.0, 1.0);
}

void main(){
   float angle;

   angle = time + 0.0; /* time + pi2 * 0.0; */
   createVertex(angle);
   texcoord = vec2(0.0, 0.0);
   EmitVertex();

   angle = time + pi2 * 3.0;
   createVertex(angle);
   texcoord = vec2(1.0, 0.0);
   EmitVertex();

   angle = time + pi2;
   createVertex(angle);
   texcoord = vec2(0.0, 1.0);
   EmitVertex();

   angle = time + pi2 * 2.0;
   createVertex(angle);
   texcoord = vec2(1.0, 1.0);
   EmitVertex();

   EndPrimitive();
}
