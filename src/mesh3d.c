#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "mesh3d.h"

typedef struct VertexCoord_{
   float x;
   float y;
   float z;
}VertexCoord;

typedef struct TexCoord_ {
   float u;
   float v;
}TexCoord;

typedef struct Index_ {
   unsigned int vertices[3];
   unsigned int texcoords[3];
   unsigned int normals[3];
}Index;

/*
   Parse the string 'line' for coordinates. The results went to the 'vertices' array.
   We use arrays instead of linked list because we would like to prevent the revers
   order of the vertices. The overload of realloc is not important.
*/

void addVertices(char *line, VertexCoord **vertices, unsigned int *count){
   *count = *count + 1;
   *vertices = realloc(*vertices, *count * sizeof(VertexCoord));
   strtok(line, " \n");

   (*vertices)[*count-1].x = atof(strtok(NULL, " \n"));
   (*vertices)[*count-1].y = atof(strtok(NULL, " \n"));
   (*vertices)[*count-1].z = atof(strtok(NULL, " \n"));
}

/*
   Parse the string 'line' for coordinates. The results went to the 'texcoords' array.
   We use arrays instead of linked list because we would like to prevent the revers
   order of the texture coordinates. The overload of realloc is not important.
*/

void addTexCoords(char *line, TexCoord **texcoords, unsigned int *count){
   *count = *count + 1;
   *texcoords = realloc(*texcoords, *count * sizeof(TexCoord));

   strtok(line, " \n");
   (*texcoords)[*count-1].u = atof(strtok(NULL, " \n"));
   (*texcoords)[*count-1].v = atof(strtok(NULL, " \n"));
}

/*
   Parse the string 'line' for coordinates. The results went to the 'indices' array.
   We use arrays instead of linked list because we would like to prevent the revers
   order of the indices. The overload of the realloc is not important.
*/

void addIndices(char *line, Index **indices, unsigned int *count){
   int   i;
   char *block, *v, *t, *n;
   char *line_save;

   *count = *count + 1;
   *indices = realloc(*indices, *count * sizeof(Index));

   strtok_r(line, " \n", &line_save);
   for(i = 0; i < 3; i++){
      block = strtok_r(NULL, " \n", &line_save);
      /* Using strsep instead of strtok, because strtok skip
         delimiters if there is no character between them */
      if( (v = strsep(&block, "/")) != NULL){
         (*indices)[*count-1].vertices[i] = atoi(v);
      }
      else{
         (*indices)[*count-1].vertices[i] = atoi(block);
      }

      if( (t = strsep(&block, "/")) != NULL){
         (*indices)[*count-1].texcoords[i] = atoi(t);
      }
      if( (n = strsep(&block, "/")) != NULL){
         (*indices)[*count-1].normals[i] = atoi(n);
      }

   }
}

/*
   After we parse the input file, we store the results in our favourite Mesh3D structure.
*/
Mesh3D *buildMesh(Index *indices,        unsigned int icount,
                  VertexCoord *vertices, unsigned int vcount,
                  VertexCoord *normals,  unsigned int ncount,
                  TexCoord *texcoords,   unsigned int tcount){
   unsigned int  i,j,vi,ni,ti;
   Mesh3D       *mesh;

   mesh = malloc(sizeof(Mesh3D));
   /* Allocate vertex identified by indices. Every indices contains
      3 vertex and every vertex contains 3 coordinates */
   mesh->vertices  = malloc(sizeof(GLfloat) * vcount * 3);
   mesh->vcount    = vcount * 3;
   mesh->normals   = malloc(sizeof(GLfloat) * vcount * 3);
   mesh->ncount    = vcount * 3;
   /* The texture coordinates contains 2 coordinates */
   mesh->texcoords = malloc(sizeof(GLfloat) * vcount * 2);
   mesh->tcount    = vcount * 2;
   /* For compatibility reason we store color coordinates.
      Colors contains 4 parts */
   mesh->colors    = malloc(sizeof(GLfloat) * vcount * 4);
   mesh->ccount    = vcount * 4;
   mesh->indices   = malloc(sizeof(GLuint) * icount * 3);
   mesh->icount    = icount * 3;

   for(i = 0; i < vcount; i++){
      mesh->vertices[i * 3 + 0]  = vertices[i].x;
      mesh->vertices[i * 3 + 1]  = vertices[i].y;
      mesh->vertices[i * 3 + 2]  = vertices[i].z;
   }

   for(i = 0; i < vcount; i++){
      mesh->colors[i*4 + 0] = 0.6f;
      mesh->colors[i*4 + 1] = 0.6f;
      mesh->colors[i*4 + 2] = 0.6f;
      mesh->colors[i*4 + 3] = 1.0f;
   }

   for(i = 0; i < icount;i++){
      for(j = 0; j < 3; j++){

         vi = indices[i].vertices[j]  - 1;
         ni = indices[i].normals[j]   - 1;
         ti = indices[i].texcoords[j] - 1;

         mesh->indices[i*3+j] = vi;
         if(ncount > 0){
            mesh->normals[vi * 3 + 0] = normals[ni].x;
            mesh->normals[vi * 3 + 1] = normals[ni].y;
            mesh->normals[vi * 3 + 2] = normals[ni].z;
         }

         if(tcount > 0){
            mesh->texcoords[vi * 2 + 0] = texcoords[ti].u;
            mesh->texcoords[vi * 2 + 1] = texcoords[ti].v;
         }
      }
   }
   return(mesh);
}

/* OpenGL draw the mesh using an integer representation */
void createOpenGLBinding(Mesh3D *mesh){
   /*TODO GL_STATIC_DRAW maybe not fit all the needs */
   glGenVertexArrays(1, &mesh->vao);
   glBindVertexArray(mesh->vao);

   glGenBuffers(5, mesh->vbo);

   /* Vertices */
   glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo[MESHVERTEXINDEX]);
   glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * mesh->vcount, mesh->vertices, GL_DYNAMIC_DRAW);

   /* Normals */
   glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo[MESHNORMALINDEX]);
   glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * mesh->ncount, mesh->normals, GL_STATIC_DRAW);

   /* Colors */
   glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo[MESHCOLORINDEX]);
   glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * mesh->ccount, mesh->colors, GL_STATIC_DRAW);

   /* Texture coordinates */
   glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo[MESHTEXINDEX]);
   glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * mesh->tcount, mesh->texcoords, GL_STATIC_DRAW);

   /* Indices */
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->vbo[MESHINDICEINDEX]);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * mesh->icount, mesh->indices, GL_STATIC_DRAW);

   glBindVertexArray(0);
}

Mesh3D *loadOBJ(const char *filename){
   FILE        *objfile;
   char         line[200];
   Mesh3D      *mesh;
   VertexCoord *vertices = NULL,
               *normals  = NULL;
   TexCoord    *texcoord = NULL;
   Index       *indices  = NULL;

   unsigned int vcount   = 0;
   unsigned int ncount   = 0;
   unsigned int tcount   = 0;
   unsigned int icount   = 0;

   /* Collect information from the file */
   objfile = fopen(filename, "r");
   while(fgets(line, 200, objfile) != NULL){
      if(line[0] == '#') continue; /* skip comments */

      if(line[0] == 'v' && line[1] == ' ') addVertices(line, &vertices, &vcount);
      if(line[0] == 'v' && line[1] == 't') addTexCoords(line, &texcoord, &tcount);
      if(line[0] == 'v' && line[1] == 'n') addVertices(line, &normals, &ncount);
      if(line[0] == 'f' && line[1] == ' ') addIndices(line, &indices, &icount);
   }
   fclose(objfile);

   /* Build mesh from the collected data */
   mesh = buildMesh(indices, icount, vertices, vcount, normals, ncount, texcoord, tcount);

   createOpenGLBinding(mesh);

   /* Free resources */
   free(vertices);
   free(texcoord);
   free(normals);
   free(indices);

   return(mesh);
}

int drawMesh3D(Mesh3D *mesh){
   glBindVertexArray(mesh->vao);
   glDrawElements(GL_TRIANGLES, mesh->icount, GL_UNSIGNED_INT, 0);
   return(0);
}

/* Release mesh resources */
int freeMesh3D(Mesh3D *mesh){

   free(mesh->vertices);
   free(mesh->colors);
   free(mesh->texcoords);
   free(mesh->normals);
   free(mesh->indices);
   glDeleteBuffers(5, mesh->vbo);
   glDeleteVertexArrays(1, &mesh->vao);
   free(mesh);

   return(0);
}

/*Change the color of the mesh */
int changeMeshColor(Mesh3D *mesh, float r, float g, float b, float a){
   unsigned int i = 0;

   while(i < mesh->ccount){
      mesh->colors[i++] = r;
      mesh->colors[i++] = g;
      mesh->colors[i++] = b;
      mesh->colors[i++] = a;
   }

   /* No problem: return 0*/
   return(0);
}

int rndMeshColor(Mesh3D *mesh){
   unsigned int i = 0;

   while(i < mesh->ccount){
      mesh->colors[i++] = drand48();
      mesh->colors[i++] = drand48();
      mesh->colors[i++] = drand48();
      mesh->colors[i++] = 1.0f;
   }

   return(0);
}
