#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <stdlib.h>

#include "face.h"
#include "shader.h"

Face* initFace(char *vssrc, char *fssrc){
   Face *face;
   GLuint loc;
   int i;

   face = malloc(sizeof(Face));
   if(face == NULL) return(NULL);

   face->verts = malloc(sizeof(GLfloat) * 640 * 480 * 2 * 3);
   if(face->verts == NULL){
      free(face);
      return(NULL);
   }

   face->indices = malloc(sizeof(GLuint) * 640 * 480 * 2);
   if(face->indices == NULL){
      free(face->verts);
      free(face);
      return(NULL);
   }

   for(i = 0; i < 640 * 480; i++){

      face->verts[i * 3 + 0] = -0.5 + ((float)i / 640.0) / 100.0;
      face->verts[i * 3 + 1] = -0.5 + (float)(i % 640) / 100.0;
      face->verts[i * 3 + 2] = 0.0;

      face->verts[(640 * 480 * 3) + i * 3 + 0] = -0.5 + ((float)i / 640.0) / 100.0;
      face->verts[(640 * 480 * 3) + i * 3 + 1] = -0.5 + (float)(i % 640) / 100.0;
      face->verts[(640 * 480 * 3) + i * 3 + 2] = 0.01;

      face->indices[i * 2 + 0] = i;
      face->indices[i * 2 + 1] = 640 * 480 + i;
   }

   face->shader_src[0] = loadShader(GL_VERTEX_SHADER, vssrc);
   face->shader_src[1] = loadShader(GL_FRAGMENT_SHADER, fssrc);
   face->shader        = createProgram(2, face->shader_src);

   glGenVertexArrays(1, &(face->vao));
   glBindVertexArray(face->vao);

   glGenBuffers(2, face->vbo);
   glBindBuffer(GL_ARRAY_BUFFER, face->vbo[0]);
   glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 640 * 480 * 2 * 3, face->verts, GL_DYNAMIC_DRAW);
   loc = glGetAttribLocation(face->shader, "vertex");
   glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, 0, 0);
   glEnableVertexAttribArray(loc);

   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, face->vbo[1]);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * 640 * 480 * 2, face->indices, GL_DYNAMIC_DRAW);

   glBindVertexArray(0);

   return(face);
}

void updateFace(unsigned char *buffer, Face *face){
   int i;
   GLfloat *verts;

   glBindBuffer(GL_ARRAY_BUFFER, face->vbo[0]);
   verts = glMapBuffer(GL_ARRAY_BUFFER, GL_READ_WRITE);

   for(i = 0; i < 640*480; i++){
      verts[(640 * 480 * 3) + i * 3 + 2] = (float)buffer[i] / 70.0;
   }

   glUnmapBuffer(GL_ARRAY_BUFFER);
   glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void drawFace(Face *face){
   glBindVertexArray(face->vao);
   glDrawElements(GL_LINES, 640*480*2, GL_UNSIGNED_INT, 0);
}

void looseFace(Face *face){
   free(face->verts);
   free(face->indices);
   free(face);
}


