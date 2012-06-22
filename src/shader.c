#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/glext.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "shader.h"

/* Create one shaders from one string */
GLuint createShader(GLenum shadertype, const char *source){
   GLuint shader;
   int    error;

   shader = glCreateShader(shadertype);
   if(shader == 0){
      return(0);
   }

   glShaderSource(shader, 1, &source, NULL);
   glCompileShader(shader);

   glGetShaderiv(shader, GL_COMPILE_STATUS, &error);
   if(error == GL_FALSE) {
      shaderError(shader, SHADER_ERROR);
      return(0);
   }

   return(shader);
}

/* Load shader source code from file */
GLuint loadShader(GLenum shadertype, const char *fn) {
   FILE *sourcefile;
   char *content = NULL;
   GLuint ret;

   int count=0;

   sourcefile = fopen(fn,"rt");

   if (sourcefile != NULL) {

     fseek(sourcefile, 0, SEEK_END);
     count = ftell(sourcefile);
     rewind(sourcefile);

     if (count > 0) {
        content = (char *)malloc(sizeof(char) * (count+1));
        count = fread(content, sizeof(char), count, sourcefile);
        content[count] = '\0';
     }
     fclose(sourcefile);
   }

   /* Try to create the shader from the source */
   ret = createShader(shadertype, content);
   if(ret == 0){
      printf("No shader for dinner\n");
   }

   if(content != NULL) free(content);
   return(ret);
}

/* Create program, attache all the shaders */
GLuint createProgram(int count, GLuint *shaders){
   GLuint prg;
   int    i;
   int    noerror;

   prg = glCreateProgram();

   for(i = 0; i < count; i++){
      glAttachShader(prg, shaders[i]);
   }

   glLinkProgram(prg);

   /* Have error during the linkage? */
   glGetProgramiv(prg, GL_LINK_STATUS, &noerror);
   if(noerror == GL_FALSE) {
      shaderError(prg, PRG_ERROR);
      return(0);
   }

   /* Validation */
   glValidateProgram(prg);
   glGetProgramiv(prg, GL_VALIDATE_STATUS, &noerror);
   if(noerror == GL_FALSE){
      shaderError(prg, PRG_ERROR);
      return(0);
   }

   return(prg);
}

/* Free some resources */
void deleteProgram(GLuint prg, int count, GLuint *shaders){
   int i;

   for(i = 0; i < count; i++){
      glDetachShader(prg, shaders[i]);
      glDeleteShader(shaders[i]);
   }

   glDeleteProgram(prg);
}



/* If some error occur this function print out error messages */
void shaderError(GLuint obj, int type){
   int   infolen = 0;
   int   outlen  = 0;
   char *infolog;

   if(type == PRG_ERROR){
      glGetProgramiv(obj, GL_INFO_LOG_LENGTH, &infolen);
   }
   else{
      glGetShaderiv(obj, GL_INFO_LOG_LENGTH, &infolen);
   }

   infolog = malloc(infolen);
   if(infolog == NULL){
      printf("No have enough memory for error message (except this one)\n");
      return;
   }

   if(type == PRG_ERROR){
      glGetProgramInfoLog(obj, infolen, &outlen, infolog);
   }
   else{
      glGetShaderInfoLog(obj, infolen, &outlen, infolog);
   }

   printf("%s\n", infolog);

   free(infolog);
}

/* Bind a shader variable to a vertex array object */
void bindVarToBuff(GLuint prg, char *varname, GLuint buffer, GLint size){
   GLint location;

   location = glGetAttribLocation(prg, varname);
   glBindBuffer(GL_ARRAY_BUFFER, buffer);
   glVertexAttribPointer(location, size, GL_FLOAT, GL_FALSE, 0, 0);
   glEnableVertexAttribArray(location);
}

