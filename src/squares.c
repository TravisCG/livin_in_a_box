#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include "squares.h"

GLuint square;
GLuint square_buffer;
GLuint background;
GLuint background_buffer;
GLuint gr;
GLuint gr_buffer;
GLuint dbgr;
GLuint dbgr_buffer;

/* Common methods */
void createSquare(GLfloat *vertex, GLuint *vao, GLuint *vbo){

   glGenVertexArrays(1, vao);
   glBindVertexArray(*vao);

   glGenBuffers(1, vbo);
   glBindBuffer(GL_ARRAY_BUFFER, *vbo);
   glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 8, vertex, GL_STATIC_DRAW);
   glVertexAttribPointer(0,2,GL_FLOAT, GL_FALSE, 0, 0);
   glEnableVertexAttribArray(0);

   glBindVertexArray(0);
}

void deleteSquare(GLuint *vao, GLuint *vbo){
   glDeleteBuffers(1, vbo);
   glDeleteVertexArrays(1, vao);
}

/* Box for the knocking scenes */

void createBox(){
   GLfloat box[] = {0.0437, 0.1375,/*Inner square */
                    0.6128, 0.1375,
                    0.6128, 0.1375,
                    0.6128, 0.9958,
                    0.6128, 0.9958,
                    0.0437, 0.9958,
                    0.0437, 0.9958,
                    0.0437, 0.1375,
                   -0.1563,-0.0625,/*Outer square */
                    0.8128,-0.0625,
                    0.8128,-0.0625,
                    0.8128, 1.1958,
                    0.8128, 1.1958,
                   -0.1563, 1.1958,
                   -0.1563, 1.1958,
                   -0.1563,-0.0625,
                    0.0437, 0.1375,/*Connections*/
                   -0.1563,-0.0625,
                    0.6128, 0.1375,
                    0.8128,-0.0625,
                    0.6128, 0.9958,
                    0.8128, 1.1958,
                    0.0437, 0.9958,
                   -0.1563, 1.1958
                    };

   glGenVertexArrays(1, &square);
   glBindVertexArray(square);

   glGenBuffers(1, &square_buffer);
   glBindBuffer(GL_ARRAY_BUFFER, square_buffer);
   glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 48, box, GL_STATIC_DRAW);
   glVertexAttribPointer(0,2,GL_FLOAT, GL_FALSE, 0, 0);
   glEnableVertexAttribArray(0);

   glBindVertexArray(0);
}

void drawBox(GLuint shader){
   glUseProgram(shader);
   glBindVertexArray(square);
   glDrawArrays(GL_LINES, 0, 24);
}

void deleteBox(){
   deleteSquare(&square, &square_buffer);
}

/* Background */

void createBackground(){
   GLfloat v[] = {-1.0, -1.0, -1.0, 1.0, 1.0, 1.0, 1.0, -1.0};
   createSquare(v, &background, &background_buffer);
}

void deleteBackground(){
   deleteSquare(&background, &background_buffer);
}

void drawBackground(){
   glBindVertexArray(background);
   glPatchParameteri(GL_PATCH_VERTICES, 4);
   glDrawArrays(GL_PATCHES, 0, 4);
}

void createDistBack(){
   GLfloat vertex[] = { 100.0, -100.0, -200.0,
                        100.0,  100.0, -200.0,
                       -100.0, -100.0, -200.0,
                       -100.0,  100.0, -200.0};

   glGenVertexArrays(1, &dbgr);
   glBindVertexArray(dbgr);

   glGenBuffers(1, &dbgr_buffer);
   glBindBuffer(GL_ARRAY_BUFFER, dbgr_buffer);
   glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 12, vertex, GL_STATIC_DRAW);
   glVertexAttribPointer(0, 3,GL_FLOAT, GL_FALSE, 0, 0);
   glEnableVertexAttribArray(0);

   glBindVertexArray(0);
}

void drawDistBack(){
   glBindVertexArray(dbgr);
   glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

/* Greetings related stuff */

void initGreetings(){
   GLfloat box[] = {3.0, 0.0, 3.0, 0.3281, 0.0, 0.0, 0.0, 0.3281};
   createSquare(box, &gr, &gr_buffer);
}

void deleteGreetings(){
   deleteSquare(&gr, &gr_buffer);
}

void drawGreetings(GLuint shader, GLuint texture, GLfloat offsetx, GLfloat offsety, GLfloat time){
   GLint loc;

   glUseProgram(shader);
   glActiveTexture(GL_TEXTURE0);
   glBindTexture(GL_TEXTURE_2D, texture);
   loc = glGetUniformLocation(shader, "img");
   glUniform1i(loc, 0);
   loc = glGetUniformLocation(shader, "offset");
   glUniform2f(loc, offsetx, offsety);
   loc = glGetUniformLocation(shader, "time");
   glUniform1f(loc, time);

   glBindVertexArray(gr);
   glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}
