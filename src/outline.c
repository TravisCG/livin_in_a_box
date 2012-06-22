#define _GNU_SOURCE
#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "outline.h"
#include "shader.h"
#include "tridlist.h"

Frames *readPyd(char *filename, int *fnum){
   FILE *pydfile;
   char *line = NULL;
   size_t size;
   Frames *frame = NULL;
   int framenum = 0;
   int contournum = 0;
   int pointnum = 0;
   char *token;
   GLfloat norm;

   pydfile = fopen(filename, "r");
   while( getline(&line, &size, pydfile) != -1){
      if(line[0] == 'F'){
         if(framenum != 0){
            frame[framenum-1].contournum = contournum;
         }
         framenum++;
         frame = realloc(frame, sizeof(Frames) * framenum);
         frame[framenum-1].contours = NULL;
         contournum = 0;
      }
      else{
         contournum++;
         frame[framenum-1].contours = realloc(frame[framenum-1].contours, sizeof(Contours) * contournum);
         frame[framenum-1].contours[contournum-1].coord = NULL;
         pointnum = 0;
         token = strtok(line, " ");
         token = strtok(NULL, " ");

         while( (token = strtok(NULL, " \n")) != NULL){
            pointnum++;
            frame[framenum-1].contours[contournum-1].coord = realloc(frame[framenum-1].contours[contournum-1].coord, sizeof(GLfloat) * pointnum);
            if(pointnum % 2 == 0) norm = 480.0; else norm = 640.0;
            frame[framenum-1].contours[contournum-1].coord[pointnum-1] = atof(token) / norm;
         }
         frame[framenum-1].contours[contournum-1].coordnum = pointnum;
      }
   }

   frame[framenum-1].contournum = contournum;
   *fnum = framenum;

   fclose(pydfile);
   free(line);
   return(frame);
}

Outline openOutline(char *filename, char *vssrc, char *fssrc){
   Outline outline;

   outline.frames = readPyd(filename, &outline.framenum);

   outline.shader_src[0] = loadShader(GL_VERTEX_SHADER, vssrc);
   outline.shader_src[1] = loadShader(GL_FRAGMENT_SHADER, fssrc);
   outline.shader        = createProgram(2, outline.shader_src);

   glGenVertexArrays(1, &outline.vao);
   glBindVertexArray(outline.vao);

   glGenBuffers(1, outline.vbo);
   glBindBuffer(GL_ARRAY_BUFFER, outline.vbo[0]);
   glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * MAX_OUTLINE_INDEX, NULL, GL_DYNAMIC_DRAW);
   glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
   glEnableVertexAttribArray(0);

   glBindVertexArray(0);

   return(outline);
}

void closeOutline(Outline outline){
   int i,j;

   for(i = 0; i < outline.framenum; i++){
      for(j = 0; j < outline.frames[i].contournum; j++){
         free(outline.frames[i].contours[j].coord);
      }
      free(outline.frames[i].contours);
   }
   free(outline.frames);

   glDeleteBuffers(1, outline.vbo);
   glDeleteVertexArrays(1, &outline.vao);
   deleteProgram(outline.shader, 2, outline.shader_src);
}

void drawOutlineFrame(Outline outline, int frame){
    GLfloat *coordinates;
    int i = 0, j = 0;

    glBindVertexArray(outline.vao);
    glUseProgram(outline.shader);

    for(i = 0; i < outline.frames[frame].contournum; i++){
       glBindBuffer(GL_ARRAY_BUFFER, outline.vbo[0]);
       coordinates = (GLfloat*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
       if(coordinates == NULL){
          fprintf(stderr, "Problem with mapping\n");
          continue;
       }
       if(outline.frames[frame].contours[i].coordnum > MAX_OUTLINE_POINTS){
          fprintf(stderr, "Too many coordinate in outline!\n");
          glUnmapBuffer(GL_ARRAY_BUFFER);
          continue;
       }
       for(j = 0; j < outline.frames[frame].contours[i].coordnum; j++){
          coordinates[j] = outline.frames[frame].contours[i].coord[j];
       }

       if(glUnmapBuffer(GL_ARRAY_BUFFER) != GL_TRUE) printf("Good bye. I will die\n");
       glDrawArrays(GL_LINE_LOOP, 0, outline.frames[frame].contours[i].coordnum / 2);

    }
}

void averageCoords(Outline outline, int frame, GLfloat *avg){
   int i,j,n = 0;
   GLfloat sumx = 0.0,sumy = 0.0;

   for(i = 0; i < outline.frames[frame].contournum; i++){
      for(j = 0; j < outline.frames[frame].contours[i].coordnum; j+=2){
         sumx += outline.frames[frame].contours[i].coord[j] * outline.frames[frame].contours[i].coord[j];
         sumy += outline.frames[frame].contours[i].coord[j] * outline.frames[frame].contours[i].coord[j+1];
         n++;
      }
   }

   avg[0] = sqrt(sumx / n);
   avg[1] = sqrt(sumy / n);
}
/*
void interpolateOutlines(Outline from, Outline to, int fromf, int tof, int fromc, int toc, float percent){
   int i,m,j = 0;
   float interx, intery;
   GLfloat *fromcoord, *tocoord, *buffer;

   fromcoord = from.frames[fromf].contours[fromc].coord;
   tocoord   = to.frames[tof].contours[toc].coord;

   glBindVertexArray(from.vao);
   glUseProgram(from.shader);
   glBindBuffer(GL_ARRAY_BUFFER, from.vbo[0]);
   buffer = (GLfloat*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);

   for(i = 0; i < from.frames[fromf].contours[fromc].coordnum; i+=2){
      m           = to.frames[tof].contours[toc].coordnum * ((float)i / (float)from.frames[fromf].contours[fromc].coordnum);
      m           = m + m % 2;
      interx      = fromcoord[i] + (tocoord[m] - fromcoord[i]) * percent;
      intery      = fromcoord[i+1] + (tocoord[m+1] - fromcoord[i+1]) * percent;
      buffer[j]   = interx;
      buffer[j+1] = intery;

      j+=2;
   }

   glUnmapBuffer(GL_ARRAY_BUFFER);
   glDrawArrays(GL_LINE_LOOP, 0, j/2);

}
*/

/* area of the triangle, but we just use the sign information */
int triArea(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2, GLfloat x3, GLfloat y3){
   float area = (x2-x1) * (y3-y1) - (x3-x1) * (y2-y1);
   return area < 0.0 ? 1 : 0;
}

/* Point is inside in polygon? */
int isInside(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2, GLfloat x3, GLfloat y3, GLfloat testx, GLfloat testy){
   int a,b,c;

   a = triArea(testx, testy, x1, y1, x2, y2);
   b = triArea(testx, testy, x2, y2, x3, y3);
   c = triArea(testx, testy, x3, y3, x1, y1);

   return( (a == b) && (a == c) );
}

/* Simple ear clipping triangulation with exploding polygon feature*/
void earClipping(TriangleDList *triangles, int *mc, GLfloat *mapped){
   int canTriangulate = 1, inside;
   Triangle *iter, *iter2;

   while(canTriangulate){
      canTriangulate = 0;
      for(iter = triangles->tail->prev; iter->prev != NULL; iter = iter->prev){
         /* Check that any point is inside in triangle */
         inside = 0;

         for(iter2 = triangles->tail; iter2 != NULL; iter2 = iter2->prev){
            if(iter2 == iter->next || iter2 == iter->prev || iter2 == iter){
               continue;
            }
            if(isInside(iter->next->x, iter->next->y, iter->x, iter->y, iter->prev->x, iter->prev->y, iter2->x, iter2->y)){
               inside = 1;
               break;
            }
         }

         /* Check the orientation of the triangle */
         if(triArea(iter->next->x, iter->next->y, iter->x, iter->y, iter->prev->x, iter->prev->y) && !inside){
            mapped[*mc + 0] = iter->next->x;
            mapped[*mc + 1] = iter->next->y;
            mapped[*mc + 2] = iter->x;
            mapped[*mc + 3] = iter->y;
            mapped[*mc + 4] = iter->prev->x;
            mapped[*mc + 5] = iter->prev->y;
            *mc += 6;
            removeElement(triangles, iter);
            canTriangulate = 1;
         }
      }
   }
}

/* Triangulate one frame from an outline */
int triangulateOutline(GLfloat *mapped, Outline outline, int frame){
   int i,j, mc = 0;
   TriangleDList triangles;

   for(i = 0; i < outline.frames[frame].contournum; i++){

      triangles.count = 0;
      triangles.tail  = NULL;
      triangles.list  = NULL;

      /* fill the duble-linked list */
      for(j = 0; j < outline.frames[frame].contours[i].coordnum; j+=2){
         addElement(&triangles, outline.frames[frame].contours[i].coord[j], outline.frames[frame].contours[i].coord[j+1]);
      }

      earClipping(&triangles, &mc, mapped);

      /* Remove the double linked list */
      deleteAll(&triangles);
   }

   return(mc);
}

/* OpenGL drawing functions for draw triangulated outline */
void drawTriangulatedOutline(Outline outline, int frame){
   GLfloat *buffer;
   int i;

   glBindVertexArray(outline.vao);
   glUseProgram(outline.shader);
   glBindBuffer(GL_ARRAY_BUFFER, outline.vbo[0]);
   buffer = (GLfloat*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);

   i = triangulateOutline(buffer, outline, frame);

   glUnmapBuffer(GL_ARRAY_BUFFER);
   glDrawArrays(GL_TRIANGLES, 0, i/2);

   /*Quick fix*/
   drawOutlineFrame(outline, frame);
}

void interpolateTriangulatedOutlines(Outline from, Outline to, int fromf, int tof, int fromc, int toc, float percent){
   int i,m,j = 0;
   float interx, intery;
   GLfloat *fromcoord, *tocoord, *buffer;
   TriangleDList triangles;

   fromcoord       = from.frames[fromf].contours[fromc].coord;
   tocoord         = to.frames[tof].contours[toc].coord;
   triangles.count = 0;
   triangles.tail  = NULL;
   triangles.list  = NULL;

   glBindVertexArray(from.vao);
   glUseProgram(from.shader);
   glBindBuffer(GL_ARRAY_BUFFER, from.vbo[0]);
   buffer = (GLfloat*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);

   for(i = 0; i < from.frames[fromf].contours[fromc].coordnum; i+=2){
      m           = to.frames[tof].contours[toc].coordnum * ((float)i / (float)from.frames[fromf].contours[fromc].coordnum);
      m           = m + m % 2;
      interx      = fromcoord[i] + (tocoord[m] - fromcoord[i]) * percent;
      intery      = fromcoord[i+1] + (tocoord[m+1] - fromcoord[i+1]) * percent;
      addElement(&triangles, interx, intery);
   }

   earClipping(&triangles, &j, buffer);

   deleteAll(&triangles);

   glUnmapBuffer(GL_ARRAY_BUFFER);
   glDrawArrays(GL_TRIANGLES, 0, j/2);
}

