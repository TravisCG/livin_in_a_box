#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <stdlib.h>
#include "shader.h"
#include "explosion.h"

GLuint explosion_vao;
GLuint explosion_vbo;
GLuint explosion_shader;
GLuint explosion_src[3];

void initExplosion(){
   GLfloat *explosion_points;
   int i;

   explosion_points = malloc(sizeof(GLfloat) * MAX_EXPLOSION);

   for(i = 0; i < MAX_EXPLOSION; i++){
      explosion_points[i] = drand48() * 2.0 - 1.0;
   }

   glGenVertexArrays(1, &explosion_vao);
   glBindVertexArray(explosion_vao);

   glGenBuffers(1, &explosion_vbo);
   glBindBuffer(GL_ARRAY_BUFFER, explosion_vbo);
   glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * MAX_EXPLOSION, explosion_points, GL_STATIC_DRAW);
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
   glEnableVertexAttribArray(0);
   glBindVertexArray(0);

   explosion_src[0]  = loadShader(GL_VERTEX_SHADER,   "shaders/explosion.vs");
   explosion_src[1]  = loadShader(GL_FRAGMENT_SHADER, "shaders/explosion.fs");
   explosion_src[2]  = loadShader(GL_GEOMETRY_SHADER, "shaders/explosion.gs");
   explosion_shader  = createProgram(3, explosion_src);

   free(explosion_points);
}

void freeExplosion(){
   glDeleteBuffers(1, &explosion_vbo);
   glDeleteVertexArrays(1, &explosion_vao);
   deleteProgram(explosion_shader, 3, explosion_src);
}

void drawExplosion(double time){
   GLuint loc;

   glUseProgram(explosion_shader);
   loc = glGetUniformLocation(explosion_shader, "time");
   glUniform1f(loc, time);

   glBindVertexArray(explosion_vao);
   glPatchParameteri(GL_PATCH_VERTICES, 1);
   glDrawArrays(GL_POINTS, 0, MAX_EXPLOSION / 3);
}
