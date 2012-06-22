#include <math.h>
#include <stdio.h>
#include <string.h>
#include "matrix.h"

void matrixMultiply4x4(float *a, float *b, float *c){
   int i,x,y;

   for(i = 0; i < 16; i++){
      x = i & 12;
      y = i % 4;
      c[i] = a[x + 0] * b[y + 0] +
             a[x + 1] * b[y + 4] +
             a[x + 2] * b[y + 8] +
             a[x + 3] * b[y + 12];
   }
}

void perspectiveMatrix(float *m, float fov, float aspect, float near, float far){
  float range;

  range = tan(fov * M_PI/360.0) * near;
  memset(m, 0, sizeof(float) * 16);
  m[0] = (2.0 * near) / ((range * aspect) - (-range * aspect));
  m[5] = (2.0 * near) / (2.0 * range);
  m[10] = -(far + near) / (far - near);
  m[11] = -1.0;
  m[14] = -(2.0 * far * near) / (far - near);
}

void translate(float *m, float x, float y, float z){
   int i;

   /* Identity matrix */
   for(i = 0; i < 16; i++){
      if( (i % 5) == 0){
         m[i] = 1.0;
      }
      else m[i] = 0.0;
   }

   m[12] = x;
   m[13] = y;
   m[14] = z;
}

void rotate(float *m, float rx, float ry, float rz){
   int i;
   /* Identity matrix */
   for(i = 0; i < 16; i++){
      if( (i % 5) == 0){
         m[i] = 1.0;
      }
      else m[i] = 0.0;
   }

   /* General rotation */
   m[0]  =  cosf(ry) * cosf(rz);
   m[4]  = -cosf(rx) * sinf(rz) + sinf(rx) * sinf(ry) * cosf(rz);
   m[8]  =  sinf(rx) * sinf(rz) + cosf(rx) * sinf(ry) * cosf(rz);
   m[12] =  0.0;
   m[1]  =  cosf(ry) * sinf(rz);
   m[5]  =  cosf(rx) * cosf(rz) + sinf(rx) * sinf(ry) * sinf(rz);
   m[9]  = -sinf(rx) * cosf(rz) + cosf(rx) * sinf(ry) * sinf(rz);
   m[13] =  0.0;
   m[2]  = -sinf(ry);
   m[6]  =  sinf(rx) * cosf(ry);
   m[10] =  cosf(rx) * cosf(ry);
   m[14] =  0.0;
   m[3]  =  0.0;
   m[7]  =  0.0;
   m[11] =  0.0;
   m[15] =  1.0;

}

void normalize(float *vector, int size){
   float d = 0.0;
   int i;

   for(i = 0; i < size; i++) d += vector[i] * vector[i];
   d = sqrtf(d);
   for(i = 0; i < size; i++) vector[i] = vector[i] / d;
}

void crossproduct3v(float *a, float *b, float *result){
   result[0] = a[1] * b[2] - a[2] * b[1];
   result[1] = a[2] * b[0] - a[0] * b[2];
   result[2] = a[0] * b[1] - a[1] * b[0];
}

float dotproduct(float *a, float *b, int size){
   int i;
   float result = 0.0;

   for(i = 0; i < size; i++) result += a[i] * b[i];
   return(result);
}

void lookAt(float *m, float *eye, float *target, float *up){
   int i;
   float n[3];
   float u[3];
   float v[3];
   float d[3];

   for(i = 0; i < 3; i++) n[i] = eye[i] - target[i];
   crossproduct3v(up, n, u);
   crossproduct3v(n, u, v);
   normalize(u, 3);
   d[0] = dotproduct(eye, u, 3) * -1.0;
   normalize(v, 3);
   d[1] = dotproduct(eye, v, 3) * -1.0;
   normalize(n, 3);
   d[2] = dotproduct(eye, n, 3) * -1.0;

   m[0]  = u[0];
   m[4]  = u[1];
   m[8]  = u[2];
   m[12]  = d[0];
   m[1]  = v[0];
   m[5]  = v[1];
   m[9]  = v[2];
   m[13]  = d[1];
   m[2]  = n[0];
   m[6]  = n[1];
   m[10] = n[2];
   m[14] = d[2];
   m[3] = 0.0;
   m[7] = 0.0;
   m[11] = 0.0;
   m[15] = 1.0;
}
