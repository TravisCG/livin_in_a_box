#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <stdlib.h>

#include "bigcube.h"
#define FRONTFACE    0
#define BACKFACE     1
#define TOPFACE      2
#define BOTTOMFACE   3
#define LEFTFACE     4
#define RIGHTFACE    5

/* Allocate memory for BigCube and create OpenGL bindings */
BigCube createBigCube(int segments){
   BigCube new;
   int vertnum, indnum, face, x, y, icount = 0;

   vertnum = segments * segments * 6 * 3;
   indnum =  segments * segments * 6 * 6;

   new.vertices = malloc(sizeof(GLfloat) * vertnum);
   new.indices  = malloc(sizeof(GLuint) * indnum);
   new.indnum   = indnum;
   new.vernum   = vertnum;
   new.segments = segments;

      for(x = 0; x < segments; x++){
         for(y = 0; y < segments; y++){
            new.vertices[ (FRONTFACE  * segments * segments * 3) + ((segments * x * 3) + (y * 3)) + 0 ] = (float)x/(float)(segments-1);
            new.vertices[ (FRONTFACE  * segments * segments * 3) + ((segments * x * 3) + (y * 3)) + 1 ] = (float)y/(float)(segments-1);
            new.vertices[ (FRONTFACE  * segments * segments * 3) + ((segments * x * 3) + (y * 3)) + 2 ] = 0.0;

            new.vertices[ (BACKFACE   * segments * segments * 3) + ((segments * x * 3) + (y * 3)) + 0 ] = (float)x/(float)(segments-1);
            new.vertices[ (BACKFACE   * segments * segments * 3) + ((segments * x * 3) + (y * 3)) + 1 ] = (float)y/(float)(segments-1);
            new.vertices[ (BACKFACE   * segments * segments * 3) + ((segments * x * 3) + (y * 3)) + 2 ] = 1.0;

            new.vertices[ (TOPFACE    * segments * segments * 3) + ((segments * x * 3) + (y * 3)) + 0 ] = (float)x/(float)(segments-1);
            new.vertices[ (TOPFACE    * segments * segments * 3) + ((segments * x * 3) + (y * 3)) + 1 ] = 1.0;
            new.vertices[ (TOPFACE    * segments * segments * 3) + ((segments * x * 3) + (y * 3)) + 2 ] = (float)y/(float)(segments-1);

            new.vertices[ (BOTTOMFACE * segments * segments * 3) + ((segments * x * 3) + (y * 3)) + 0 ] = (float)x/(float)(segments-1);
            new.vertices[ (BOTTOMFACE * segments * segments * 3) + ((segments * x * 3) + (y * 3)) + 1 ] = 0.0;
            new.vertices[ (BOTTOMFACE * segments * segments * 3) + ((segments * x * 3) + (y * 3)) + 2 ] = (float)y/(float)(segments-1);

            new.vertices[ (LEFTFACE   * segments * segments * 3) + ((segments * x * 3) + (y * 3)) + 0 ] = 0.0;
            new.vertices[ (LEFTFACE   * segments * segments * 3) + ((segments * x * 3) + (y * 3)) + 1 ] = (float)y/(float)(segments-1);
            new.vertices[ (LEFTFACE   * segments * segments * 3) + ((segments * x * 3) + (y * 3)) + 2 ] = (float)x/(float)(segments-1);

            new.vertices[ (RIGHTFACE  * segments * segments * 3) + ((segments * x * 3) + (y * 3)) + 0 ] = 1.0;
            new.vertices[ (RIGHTFACE  * segments * segments * 3) + ((segments * x * 3) + (y * 3)) + 1 ] = (float)y/(float)(segments-1);
            new.vertices[ (RIGHTFACE  * segments * segments * 3) + ((segments * x * 3) + (y * 3)) + 2 ] = (float)x/(float)(segments-1);
         }
      }

   for(face = 0; face < 6; face++){
      for( x = 0; x < segments - 1; x++){
         for(y = 0; y < segments - 1; y++){
            new.indices[icount] = face * segments * segments + segments * x + y;
            icount++;
            new.indices[icount] = face * segments * segments + segments * (x+1) + y;
            icount++;
            new.indices[icount] = face * segments * segments + segments * (x+1) + y + 1;
            icount++;

            new.indices[icount] = face * segments * segments + segments * x + y;
            icount++;
            new.indices[icount] = face * segments * segments + segments * (x+1) + y + 1;
            icount++;
            new.indices[icount] = face * segments * segments + segments * x + y + 1;
            icount++;
         }
      }
   }

   glGenVertexArrays(1, &new.vao);
   glBindVertexArray(new.vao);

   glGenBuffers(2, new.vbo);
   glBindBuffer(GL_ARRAY_BUFFER, new.vbo[0]);
   glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertnum, new.vertices, GL_DYNAMIC_DRAW);
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
   glEnableVertexAttribArray(0);

   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, new.vbo[1]);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indnum, new.indices, GL_STATIC_DRAW);

   return(new);
}

/* Release resources of BigCube */
void freeBigCube(BigCube bc){
   free(bc.indices);
   free(bc.vertices);
   glDeleteBuffers(2, bc.vbo);
   glDeleteVertexArrays(1, &bc.vao);
}

void drawBigCube(BigCube bc){
   glBindVertexArray(bc.vao);
   glDrawElements(GL_TRIANGLES, bc.indnum, GL_UNSIGNED_INT, 0);
}

void updateBigCube(unsigned char *buffer, BigCube *bc){
   GLfloat *verts;
   int i,x,y;

   glBindBuffer(GL_ARRAY_BUFFER, bc->vbo[0]);
   verts = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
   for(i = 0; i < 640*480; i++){
      x = i % 640;
      y = i / 640;
      verts[ (BACKFACE  * bc->segments * bc->segments * 3) + ((bc->segments * x * 3) + (y * 3)) + 2 ] = (float)buffer[i] / 240.0;
   }
   glUnmapBuffer(GL_ARRAY_BUFFER);
   glBindBuffer(GL_ARRAY_BUFFER, 0);
}
