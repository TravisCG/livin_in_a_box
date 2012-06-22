#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "livin.h"
#include "cg_time.h"
#include "cg_x11.h"
#include "shader.h"
#include "matrix.h"
#include "mesh3d.h"
#include "texture.h"
#include "depthvideo.h"
#include "face.h"
#include "outline.h"
#include "squares.h"
#include "explosion.h"
#include "bigcube.h"

#define MAXKNOCKINGMAN 20
#define LETTERNUM 10

DepthVideo head;
DepthVideo matat;

float pmatrix[16];

Face *face;
Face *armface;

Outline knock;
Outline jump;
Outline hang;
Outline run;
Outline falling;
Outline tiger;
Outline hop;
Outline flip;
Outline katf;
Outline run2;
Outline catwheel;
Outline tbill;
Outline door;
Outline trap;
Outline fractalme;

GLuint background_src[5];
GLuint background_shader;
GLuint title_src[5];
GLuint title_shader;
GLuint credits_src[5];
GLuint credits_shader;
GLuint letter_src[2];
GLuint letter_shader;
GLuint scroll_src[5];
GLuint scroll_shader;
GLuint cube_src[3];
GLuint cube_shader;
GLuint gr_src[2];
GLuint gr_shader;
GLuint dbgr_src[2];
GLuint dbgr_shader;
GLuint bigcube_src[2];
GLuint bigcube_shader;

GLuint titletex;
GLuint greentex;
GLuint creditstex;
GLuint bgalphatex;
GLuint whitetex;
GLuint greetingstex[12];
GLuint icubetex;
GLuint headouttex;
GLuint chesstex;
GLuint atpartytex;

Mesh3D *letters[LETTERNUM];
Mesh3D *cube;
BigCube bigcube;

int matatpos = 0;

float knockingmans[MAXKNOCKINGMAN * 3];

void InitLivin(int width, int height){
   int i;
   char letterpath[] = "data/x.obj"; /* character mesh mask */
   char letterlist[LETTERNUM] = "bceginrsty"; /* Cybernetic genetics characters */
   char greetingspath[21];

   /* OpenGL related initialization */
   glClearColor(0.0, 0.0, 0.0, 0.0);
   glEnable(GL_DEPTH_TEST);
   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   
   perspectiveMatrix(pmatrix, 35.0, (float)width/(float)height, 0.1, 1500.0);

   /* Demo related initialization */
   head      = openDepthVideo("data/head.dv");
   matat     = openDepthVideo("data/matat.dv");
   face      = initFace("shaders/face.vs", "shaders/face.fs");
   armface   = initFace("shaders/face.vs", "shaders/face.fs");
   knock     = openOutline("data/knock.pyd",           "shaders/knock.vs",   "shaders/knock.fs");
   jump      = openOutline("data/jump.pyd",            "shaders/jump.vs",    "shaders/outline.fs");
   hang      = openOutline("data/hanging.pyd",         "shaders/hang.vs",    "shaders/outline.fs");
   run       = openOutline("data/run.pyd",             "shaders/run.vs",     "shaders/outline.fs");
   falling   = openOutline("data/falling_letters.pyd", "shaders/run.vs",     "shaders/outline.fs");
   door      = openOutline("data/door.pyd",            "shaders/door.vs",    "shaders/door.fs");
   trap      = openOutline("data/trap.pyd",            "shaders/trap.vs",    "shaders/outline.fs");
   fractalme = openOutline("data/fractalme.pyd",       "shaders/frme.vs",    "shaders/frme.fs");
   /* Acrobatic outlines */
   tiger     = openOutline("data/tiger.pyd",           "shaders/acrobat.vs", "shaders/outline.fs");
   hop       = openOutline("data/hop.pyd",             "shaders/acrobat.vs", "shaders/outline.fs");
   katf      = openOutline("data/kezenatfordulas.pyd", "shaders/acrobat.vs", "shaders/outline.fs");
   flip      = openOutline("data/flip.pyd",            "shaders/acrobat.vs", "shaders/outline.fs");
   run2      = openOutline("data/run2.pyd",            "shaders/acrobat.vs", "shaders/outline.fs");
   catwheel  = openOutline("data/catwheel.pyd",        "shaders/acrobat.vs", "shaders/outline.fs");
   tbill     = openOutline("data/tarkobillenes.pyd",   "shaders/acrobat.vs", "shaders/outline.fs");

   createBox();
   createBackground();
   initGreetings();
   createDistBack();
   bigcube = createBigCube(600);

   titletex   = loadPNGTexture("data/title.png");
   greentex   = loadPNGTexture("data/green.png");
   creditstex = loadPNGTexture("data/credits.png");
   bgalphatex = loadPNGTexture("data/bg_alpha.png");
   whitetex   = loadPNGTexture("data/white.png");
   icubetex   = loadPNGTexture("data/innercube.png");
   headouttex = loadPNGTexture("data/headout.png");
   chesstex   = loadPNGTexture("data/chessboard.png");
   atpartytex = loadPNGTexture("data/at_party.png");

   for(i = 0; i < 12; i++){
      sprintf(greetingspath, "data/greetings%d.png", i+1);
      greetingstex[i] = loadPNGTexture(greetingspath);
   }

   for(i = 0; i < MAXKNOCKINGMAN; i++){
      knockingmans[i * 3 + 0] = drand48() * 4.0 + 8.0;
      knockingmans[i * 3 + 1] = drand48() * 6.0 + 1.0;
      knockingmans[i * 3 + 2] = drand48();
   }

   /* Shaders */
   background_src[0] = loadShader(GL_VERTEX_SHADER,          "shaders/background.vs");
   background_src[1] = loadShader(GL_FRAGMENT_SHADER,        "shaders/background.fs");
   background_src[2] = loadShader(GL_TESS_EVALUATION_SHADER, "shaders/background.te");
   background_src[3] = loadShader(GL_TESS_CONTROL_SHADER,    "shaders/background.tc");
   background_src[4] = loadShader(GL_GEOMETRY_SHADER,        "shaders/background.gs");
   background_shader = createProgram(5, background_src);

   title_src[0]      = background_src[0];
   title_src[1]      = loadShader(GL_FRAGMENT_SHADER, "shaders/title.fs");
   title_src[2]      = background_src[2];
   title_src[3]      = background_src[3];
   title_src[4]      = background_src[4];
   title_shader      = createProgram(5, title_src);

   scroll_src[0]     = background_src[0];
   scroll_src[1]     = loadShader(GL_FRAGMENT_SHADER, "shaders/scroll.fs");
   scroll_src[2]     = background_src[2];
   scroll_src[3]     = background_src[3];
   scroll_src[4]     = background_src[4];
   scroll_shader     = createProgram(5, scroll_src);

   credits_src[0]    = background_src[0];
   credits_src[1]    = loadShader(GL_FRAGMENT_SHADER, "shaders/credits.fs");
   credits_src[2]    = background_src[2];
   credits_src[3]    = background_src[3];
   credits_src[4]    = loadShader(GL_GEOMETRY_SHADER, "shaders/credits.gs");
   credits_shader    = createProgram(5, credits_src);

   letter_src[0]     = loadShader(GL_VERTEX_SHADER,   "shaders/letter.vs");
   letter_src[1]     = loadShader(GL_FRAGMENT_SHADER, "shaders/letter.fs");
   letter_shader     = createProgram(2, letter_src);

   cube_src[0]       = loadShader(GL_VERTEX_SHADER,   "shaders/cube.vs");
   cube_src[1]       = loadShader(GL_FRAGMENT_SHADER, "shaders/cube.fs");
   cube_src[2]       = loadShader(GL_GEOMETRY_SHADER, "shaders/cube.gs");
   cube_shader       = createProgram(3, cube_src);

   gr_src[0]         = loadShader(GL_VERTEX_SHADER,   "shaders/greetings.vs");
   gr_src[1]         = loadShader(GL_FRAGMENT_SHADER, "shaders/greetings.fs");
   gr_shader         = createProgram(2, gr_src);

   dbgr_src[0]       = loadShader(GL_VERTEX_SHADER,   "shaders/dbgr.vs");
   dbgr_src[1]       = loadShader(GL_FRAGMENT_SHADER, "shaders/dbgr.fs");
   dbgr_shader       = createProgram(2, dbgr_src);

   bigcube_src[0]    = loadShader(GL_VERTEX_SHADER,   "shaders/bigcube.vs");
   bigcube_src[1]    = loadShader(GL_FRAGMENT_SHADER, "shaders/bigcube.fs");
   bigcube_shader    = createProgram(2, bigcube_src);

   /* Letters */
   for(i = 0; i < LETTERNUM; i++){
      letterpath[5] = letterlist[i];
      letters[i] = loadOBJ(letterpath);

      glBindVertexArray(letters[i]->vao);
      bindVarToBuff(letter_shader, "vertex", letters[i]->vbo[MESHVERTEXINDEX], 3);
      bindVarToBuff(letter_shader, "normal", letters[i]->vbo[MESHNORMALINDEX], 3);
      bindVarToBuff(letter_shader, "tcoord", letters[i]->vbo[MESHTEXINDEX],    2);
   }

   cube = loadOBJ("data/cube.obj");
   glBindVertexArray(cube->vao);
   bindVarToBuff(cube_shader, "vertex", cube->vbo[MESHVERTEXINDEX], 3);
   bindVarToBuff(cube_shader, "normal", cube->vbo[MESHNORMALINDEX], 3);
   bindVarToBuff(cube_shader, "tcoord", cube->vbo[MESHTEXINDEX],    2);

   initExplosion();

   startTime();
}

void drawKnockingMan(int frame, double time, float xoffset, float yoffset){
   GLuint loc;

   glUseProgram(knock.shader);
   loc = glGetUniformLocation(knock.shader, "growfactor");
   glUniform1f(loc, time);
   loc = glGetUniformLocation(knock.shader, "offset");
   glUniform2f(loc, xoffset, yoffset);
   
   drawOutlineFrame(knock, frame);
}

void drawJumpingMan(double time, float xoffset, float yoffset){
   GLuint loc;
   int frame;
   const float deadline = 3.5;

   if(time < deadline){
      frame = (int)(time / OUTLINE_FPS) % jump.framenum;
   }
   else{
      frame = (int)(deadline / OUTLINE_FPS) % jump.framenum;
      yoffset -= (time-deadline) * (time-deadline);
      xoffset -= (time-deadline)/10.0;
   }

   jump.shader = knock.shader;
   glUseProgram(jump.shader);
   loc = glGetUniformLocation(jump.shader, "growfactor");
   glUniform1f(loc, 8.0);
   loc = glGetUniformLocation(jump.shader, "offset");
   glUniform2f(loc, xoffset - 0.13, yoffset - 0.02);

   drawOutlineFrame(jump, frame);
}

void drawLetters(double time){
   GLuint loc;
   int i;
   int text[33] = {1,9,0,2,6,5,2,8,4,1,3,2,5,2,8,4,1,7,
                   2,6,1,5,3,6,4,0,3,8,3,5,7,2,1};
   float offset[33] = {-65.0, -57.0, -49.0, -41.0, -33.0, -25.0, -17.0, -9.0, -4.0, 1.0, 15.0, 23.0, 31.0, 39.0, 47.0, 52.0, 57.0, 65.0,
                       -4.0, 1.0, -4.0, 1.0, -47.0, -23.0, -8.0, 15.0, 20.0, 30.0, 40.0, 45.0, -47.0, -52.0, -60.0};
   float start[33] = {5.21,7.21,7.63,6.52,8.21,8.56,4.0758,4.5221,4.7203,2.4149,9.2415,7.8153,8.84,5.67,7.47,5.34,9.00,6.23,
                      10.3739, 12.076, 13.666,16.851, 11.24, 12.73, 14.32, 15.42, 16.21, 12.97, 14.89, 16.11, 11.64, 16.43, 16.65};
   float tmatrix[16];
   float rmatrix[16];
   float camera[16];
   float ypos, basey;

   glUseProgram(letter_shader);
   loc = glGetUniformLocation(letter_shader, "pmatrix");
   glUniformMatrix4fv(loc, 1, GL_FALSE, pmatrix);

   for(i = 0; i < 33; i++){
      if(i < 18){
         basey = 40.0;
      }
      else{
         basey = 60.0;
      }

      if(time > start[i]){
         ypos = basey - 9.822 * (time - start[i]) * (time - start[i]);
      }
      else{
         ypos = basey;
      }

      translate(tmatrix, offset[i], ypos, -170.0);
      ypos -= basey;
      rotate(rmatrix, M_PI / -2.0 + (ypos / 40.0), 0.0 + (ypos / 40.0), 0.0);
      matrixMultiply4x4(rmatrix, tmatrix, camera);
      loc = glGetUniformLocation(letter_shader, "camera");
      glUniformMatrix4fv(loc, 1, GL_FALSE, camera);
      drawMesh3D(letters[text[i]]);
   }
}

void DistantBackground(){
   GLuint loc;

   glUseProgram(dbgr_shader);
   glActiveTexture(GL_TEXTURE0);
   glBindTexture(GL_TEXTURE_2D, chesstex);
   loc = glGetUniformLocation(dbgr_shader, "image");
   glUniform1i(loc, 0);
   loc = glGetUniformLocation(dbgr_shader, "pmatrix");
   glUniformMatrix4fv(loc, 1, GL_FALSE, pmatrix);
   
   drawDistBack();

}

/* 
    ******
    Scenes
    ******
*/

/* The last scene. I have made it first. :-)*/
void Headbreak(double time){
   static int i = 0;
   static float rot = 0.0;
   static double prev_time;
   GLuint loc;
   float tmatrix[16];
   float rmatrix[16];
   float mmatrix[16];

   translate(tmatrix, -0.4, 3.0, -7.0);
   rotate(rmatrix, 0.0, 0.0, 4.1);
   matrixMultiply4x4(rmatrix, tmatrix, mmatrix);
   /* 30fps video playback */
   if(time - prev_time > 0.03){
      getNextFrame(head);
      updateFace(head.buffer, face);
      prev_time = time;
      i++;
   }

   glUseProgram(face->shader);
   loc = glGetUniformLocation(face->shader, "pmatrix");
   glUniformMatrix4fv(loc, 1, GL_FALSE, pmatrix);
   loc = glGetUniformLocation(face->shader, "mmatrix");
   glUniformMatrix4fv(loc, 1, GL_FALSE, mmatrix);
   drawFace(face);

   rot += 0.05;
   if(i > 87){
      rewindDepthVideo(head);
      i = 1;
   }
}

/* First scene. Knocking in the wall */
void KnockKnock(double time){
   int frame = 0;
   GLint loc;

   frame = (int)(time / OUTLINE_FPS) % knock.framenum;

   glUseProgram(title_shader);
   glBindTexture(GL_TEXTURE_2D, titletex);
   glActiveTexture(GL_TEXTURE0);
   loc = glGetUniformLocation(title_shader, "image");
   glUniform1i(loc, 0);

   loc = glGetUniformLocation(title_shader, "wtime");
   glUniform1f(loc, time);
   loc = glGetUniformLocation(title_shader, "distfactor");
   glUniform1f(loc, 0.0);
   loc = glGetUniformLocation(title_shader, "voffset");
   glUniform2f(loc, 0.0, 0.0);

   loc = glGetUniformLocation(title_shader, "time");
   if(time > 14.04){
      glUniform1f(loc, time - 14.04);
   }
   else{
      glUniform1f(loc, 0.0f);
   }
   drawBackground();

   time = time < 14.0 ? time : 14.0;

   drawKnockingMan(frame, time, 0.0, 0.0);
   drawBox(knock.shader);
}

/* Jump out from the box */
void JumpOut(double time){
   int frame = 0;
   GLuint loc;

   glUseProgram(background_shader);
   glActiveTexture(GL_TEXTURE0);
   glBindTexture(GL_TEXTURE_2D, titletex);
   loc = glGetUniformLocation(background_shader, "image");
   glUniform1i(loc, 0);
   loc = glGetUniformLocation(background_shader, "wtime");
   glUniform1f(loc, time);
   loc = glGetUniformLocation(background_shader, "distfactor");
   glUniform1f(loc, 0.0);
   loc = glGetUniformLocation(background_shader, "voffset");
   glUniform2f(loc, 0.0, 0.0);

   drawBackground();

   frame = (int)(time / OUTLINE_FPS);
   if(frame >= jump.framenum) return;
   drawTriangulatedOutline(jump, frame);
}

/* Running scene */
void RunForLife(double time){
   int frame = 0;
   int kframe = 0;
   int i;
   GLfloat avg[2];
   GLuint loc;
   float grow, x, y, ftime;

   frame  = (int)(time / OUTLINE_FPS) % run.framenum;
   kframe = 8 + ((int)(time / OUTLINE_FPS) % 46);

   /* Calculate quadric average of the points */
   averageCoords(run, frame, avg);

   /* Draw running man */
   glUseProgram(run.shader);
   loc = glGetUniformLocation(run.shader, "avg");
   glUniform1f(loc, avg[0]);
   loc = glGetUniformLocation(run.shader, "offset");
   glUniform2f(loc, -0.5, -1.0);

   drawTriangulatedOutline(run, frame);
   for(i = 0; i < 5; i++){
      averageCoords(run, abs(frame - i), avg);
      loc = glGetUniformLocation(run.shader, "avg");
      glUniform1f(loc, avg[0]);
      loc = glGetUniformLocation(run.shader, "offset");
      glUniform2f(loc, -0.5 - (float)i / 50.0, -1.0);
      drawOutlineFrame(run, abs(frame - i));
   }

   /* Draw the knocking persons */
   for(i = 0; i < MAXKNOCKINGMAN; i++){
      ftime = time - (double)( (int)(time / 8) * 8.0);
      grow = knockingmans[i * 3];
      x    = knockingmans[i * 3 + 1] - ftime * (grow / 10.0);
      y    = knockingmans[i * 3 + 2];

      drawKnockingMan(kframe + i, grow, x, y);
      drawBox(knock.shader);
   }

   /* Draw background */
   glUseProgram(scroll_shader);
   glActiveTexture(GL_TEXTURE0);
   glBindTexture(GL_TEXTURE_2D, greentex);
   loc = glGetUniformLocation(scroll_shader, "image");
   glUniform1i(loc, 0);
   loc = glGetUniformLocation(scroll_shader, "wtime");
   glUniform1f(loc, time);
   loc = glGetUniformLocation(scroll_shader, "distfactor");
   glUniform1f(loc, 0.0);
   loc = glGetUniformLocation(scroll_shader, "voffset");
   glUniform2f(loc, 0.0, 0.0);

   drawBackground();
}

void Credits(double time){
   GLuint loc;

   glUseProgram(credits_shader);
   glActiveTexture(GL_TEXTURE0);
   glBindTexture(GL_TEXTURE_2D, creditstex);
   loc = glGetUniformLocation(credits_shader, "image");
   glUniform1i(loc, 0);
   loc = glGetUniformLocation(credits_shader, "time");
   time = (time < 65.12) ? 0.0 : time - 65.12;
   glUniform1f(loc, time);
   loc = glGetUniformLocation(credits_shader, "distfactor");
   glUniform1f(loc, 0.0);
   loc = glGetUniformLocation(credits_shader, "voffset");
   glUniform2f(loc, 0.0, 0.0);

   drawBackground();
}

void Greetings(double time){
   static Outline *outline = &run2;
   GLfloat avg[2] = {0.0, 0.0};
   static int frame = 0, loopnum = 0;
   static double prev_time = 0.0;
   GLuint loc;
   int i;

   if(time - prev_time > OUTLINE_FPS){
      frame++;
      prev_time = time;
      if(frame >= outline->framenum){
         frame = 0;
         loopnum++;
      }
   }

   /* Choose acrobatic movements */
   switch(loopnum){
      case 8:
         outline = &tiger;
         break;
      case 16:
         outline = &katf;
         break;
      case 24:
         outline = &hop;
         break;
      case 32:
         outline = &tbill;
         break;
      case 40:
         outline = &catwheel;
         break;
      case 45:
         outline = &flip;
         break;
      default:
         outline = &run2;
   };

   averageCoords(*outline, frame, avg);


   /* Draw running man */
   glUseProgram(outline->shader);
   loc = glGetUniformLocation(outline->shader, "avg");
   glUniform2f(loc, avg[0], avg[1]);
   loc = glGetUniformLocation(outline->shader, "offset");
   glUniform2f(loc, 0.0, -1.5);

   drawTriangulatedOutline(*outline, frame);

   glUseProgram(scroll_shader);
   glActiveTexture(GL_TEXTURE0);
   glBindTexture(GL_TEXTURE_2D, bgalphatex);
   loc = glGetUniformLocation(scroll_shader, "image");
   glUniform1i(loc, 0);
   loc = glGetUniformLocation(scroll_shader, "wtime");
   glUniform1f(loc, time);
   loc = glGetUniformLocation(scroll_shader, "distfactor");
   glUniform1f(loc, 1.0);
   loc = glGetUniformLocation(background_shader, "voffset");
   glUniform2f(loc, 0.0, 0.0);

   drawBackground();

   for(i = 0; i < 12; i++){
      drawGreetings(gr_shader, greetingstex[i], 52.0 / 12.0 * (float)(i) , -0.7, time);
   }

}

void HangingFruite(double time){
   GLuint loc;
   int frame = 0;
   static double prev_time = 0.0;
   float ypos = -0.13;
   float p = 0.0;

   /* Draw the letters */
   drawLetters(0.0);
   frame = time / OUTLINE_FPS;
   /* Hanging or falling */
   if(frame > hang.framenum - 1){
      frame = hang.framenum - 1;
      ypos = -0.13 + 9.822 * (time - prev_time) * (time - prev_time); /* Like real gravity */
      p = ypos / 1.14;
      if(p < 0.0) p = 0.0;
   }
   else{
      prev_time = time;
   }

   /* Draw the poor guy */
   glUseProgram(hang.shader);
   loc = glGetUniformLocation(hang.shader, "offset");
   glUniform2f(loc, -0.4, ypos);

   if(frame == hang.framenum -1){
      interpolateTriangulatedOutlines(hang, falling, hang.framenum-1, 0, 1, 1, p);
   }
   else{
      drawTriangulatedOutline(hang, frame);
   }

   DistantBackground();
}

void WatchOutForTheLetters(double time){
   int frame = 0;
   GLuint loc;

   frame = time / OUTLINE_FPS;
   if(frame > falling.framenum-1) frame = 0;

   drawLetters(time);

   glUseProgram(falling.shader);
   loc = glGetUniformLocation(falling.shader, "offset");
   glUniform2f(loc, -0.9, -1.14);
   drawTriangulatedOutline(falling, frame);

   DistantBackground();
}

void ProceduralScenes(double time){
   int frame = 0;
   int i,j;
   float x,y;
   GLuint loc;
   const float deadline = 10.0;
   
   if(time < deadline){
      frame = (int)(time / OUTLINE_FPS) % knock.framenum;
   }
   else{
      frame = (int)(deadline / OUTLINE_FPS) % knock.framenum;
   }

   for(i = 0; i < knock.frames[frame].contournum; i++){
      for(j = 0; j < knock.frames[frame].contours[i].coordnum-1; j+=2){
         x = (knock.frames[frame].contours[i].coord[j]) * 2.0;
         y = (0.6 - knock.frames[frame].contours[i].coord[j+1]) * 2.0;
         if(time > deadline){
            drawJumpingMan((time - deadline) / fabs(y), x, y);
            drawJumpingMan((time - deadline) / fabs(y), 0.0 - x, y);
         }
         else{
            drawKnockingMan(frame, 2.0, x,y);
            drawKnockingMan(frame, 2.0, 0.0 - x, y);
         }
      }
   }


   glUseProgram(background_shader);
   glActiveTexture(GL_TEXTURE0);
   glBindTexture(GL_TEXTURE_2D, greentex);
   loc = glGetUniformLocation(background_shader, "image");
   glUniform1i(loc, 0);
   loc = glGetUniformLocation(background_shader, "wtime");
   glUniform1f(loc, time);
   loc = glGetUniformLocation(background_shader, "distfactor");
   glUniform1f(loc, 0.0);
   loc = glGetUniformLocation(background_shader, "voffset");
   glUniform2f(loc, 0.0, 0.0);


   drawBackground();

}

void CubeTunnel(double time){
   GLint loc;

   glUseProgram(cube_shader);
   glActiveTexture(GL_TEXTURE0);
   glBindTexture(GL_TEXTURE_2D, whitetex);
   loc = glGetUniformLocation(cube_shader, "time");
   glUniform1f(loc, time);
   loc = glGetUniformLocation(cube_shader, "img");
   glUniform1i(loc, 0);
   loc = glGetUniformLocation(cube_shader, "pmatrix");
   glUniformMatrix4fv(loc, 1, GL_FALSE, pmatrix);
   drawMesh3D(cube);
}

void OpenTheDoor(double time){
   int frame = 0;
   GLint loc;

   glUseProgram(door.shader);
   frame = (int)(time / 0.1102) % door.framenum;
   drawTriangulatedOutline(door, frame);

   glUseProgram(background_shader);
   glActiveTexture(GL_TEXTURE0);
   glBindTexture(GL_TEXTURE_2D, headouttex);
   loc = glGetUniformLocation(background_shader, "image");
   glUniform1i(loc, 0);
   loc = glGetUniformLocation(background_shader, "wtime");
   glUniform1f(loc, time);
   loc = glGetUniformLocation(background_shader, "distfactor");
   glUniform1f(loc, 0.0);
   loc = glGetUniformLocation(background_shader, "voffset");
   glUniform2f(loc, 0.0, 0.0);

   drawBackground();
}

void Trap(double time){
   int frame = 0;
   GLint loc;
   float offset;

   glUseProgram(trap.shader);
   loc = glGetUniformLocation(trap.shader, "offset");
   offset = time < 22.94 ? 0.0 : (time - 22.94) * (time - 22.94) * 9.822;
   glUniform1f(loc, offset);
   frame = (int)(time / OUTLINE_FPS) % trap.framenum;
   drawTriangulatedOutline(trap, frame);

   glUseProgram(background_shader);
   glActiveTexture(GL_TEXTURE0);
   glBindTexture(GL_TEXTURE_2D, icubetex);
   loc = glGetUniformLocation(background_shader, "image");
   glUniform1i(loc, 0);
   loc = glGetUniformLocation(background_shader, "wtime");
   glUniform1f(loc, time);
   loc = glGetUniformLocation(background_shader, "distfactor");
   glUniform1f(loc, 0.0);
   loc = glGetUniformLocation(background_shader, "voffset");
   glUniform2f(loc, 0.0, 0.0);

   drawBackground();
}

void FractalMe(double time){
   GLint loc;
   int frame = 0, bigframe = 30, bigframe2 = 30;
   float x, y, nextx, nexty;
   float particlerot = 0.0, particlexzoom = 2.94, particleyzoom = 2.95, particlexoffset = 1.11, particleyoffset = 1.77;
   float bigoffsetx = 1.3, bigoffsety = 1.7;
   float bgoffsety = 0.0;
   int i,j;

   if(time > 0.0 && time < 2.31){
      bigframe = (int)(time / OUTLINE_FPS) % 30;
      bigframe2 = bigframe;
      frame = 0;
   }
   if(time > 2.31 && time < 14.9){
      frame = (int)((time - 2.31) / OUTLINE_FPS) % fractalme.framenum;
   }
   if(time > 14.8){
      frame = fractalme.framenum-1;
      particlerot = (time-14.8) / 2.0;
      particlexzoom += (time-14.8);
      particleyzoom += (time-14.8);
      particlexoffset += (time-14.8)/2.4;
      particleyoffset += (time-14.8)/2.0;
   }
   if(time > 17.0 && time < 27.69){
      bigframe = (int)((time - 14.69) / OUTLINE_FPS) % fractalme.framenum;
   }
   if(time > 27.69){
      bigoffsety += (time - 27.69);
      bgoffsety -= (time - 27.69);
   }

   glUseProgram(fractalme.shader);

   loc = glGetUniformLocation(fractalme.shader, "size");
   glUniform1f(loc, 3.0);
   loc = glGetUniformLocation(fractalme.shader, "offset");
   glUniform2f(loc, bigoffsetx, bigoffsety);
   loc = glGetUniformLocation(fractalme.shader, "angle");
   glUniform1f(loc, M_PI);
   drawTriangulatedOutline(fractalme, bigframe);

   for(i = 0; i < fractalme.frames[bigframe2].contournum; i++){
      for(j = 0; j < fractalme.frames[bigframe2].contours[i].coordnum; j += 2){

         x = fractalme.frames[bigframe2].contours[i].coord[j] * particlexzoom - particlexoffset;
         y = particleyoffset - fractalme.frames[bigframe2].contours[i].coord[j+1] * particleyzoom;
         if(j+3 < fractalme.frames[bigframe2].contours[i].coordnum){
            nextx = fractalme.frames[bigframe2].contours[i].coord[j+2] * particlexzoom - particlexoffset;
            nexty = particleyoffset - fractalme.frames[bigframe2].contours[i].coord[j+3] * particleyzoom;
         }
         else{
            nextx = fractalme.frames[bigframe2].contours[i].coord[0] * particlexzoom - particlexoffset;
            nexty = particleyoffset - fractalme.frames[bigframe2].contours[i].coord[1] * particleyzoom; 
         }

         loc = glGetUniformLocation(fractalme.shader, "offset");
         glUniform2f(loc, x, y);
         loc = glGetUniformLocation(fractalme.shader, "size");
         glUniform1f(loc, 0.1);
         loc = glGetUniformLocation(fractalme.shader, "angle");
         glUniform1f(loc, atan2((nexty - y), (nextx - x)) + particlerot);
         loc = glGetUniformLocation(fractalme.shader, "center");
         glUniform2f(loc, 0.0, 0.1);
         drawTriangulatedOutline(fractalme, frame);

      }
   }

   /* Draw background */
   glUseProgram(background_shader);
   glActiveTexture(GL_TEXTURE0);
   glBindTexture(GL_TEXTURE_2D, greentex);
   loc = glGetUniformLocation(background_shader, "image");
   glUniform1i(loc, 0);
   loc = glGetUniformLocation(background_shader, "wtime");
   glUniform1f(loc, time);
   loc = glGetUniformLocation(background_shader, "distfactor");
   glUniform1f(loc, 0.0);
   loc = glGetUniformLocation(background_shader, "voffset");
   glUniform2f(loc, 0.0, bgoffsety);

   drawBackground();
}

void Matatas(double time){
   static double prev_time = 0.0;
   GLuint loc;
   float tmatrix[16];
   float rmatrix[16];
   float mmatrix[16];

   translate(tmatrix, -0.4, 3.0, -7.0);
   rotate(rmatrix, 0.0, 0.0, 4.1);
   matrixMultiply4x4(rmatrix, tmatrix, mmatrix);
   /* 30fps video playback */
   if(time - prev_time > 0.03){
      getNextFrame(matat);
      updateFace(matat.buffer, armface);
      prev_time = time;
      matatpos++;
   }

   glUseProgram(armface->shader);
   loc = glGetUniformLocation(armface->shader, "pmatrix");
   glUniformMatrix4fv(loc, 1, GL_FALSE, pmatrix);
   loc = glGetUniformLocation(armface->shader, "mmatrix");
   glUniformMatrix4fv(loc, 1, GL_FALSE, mmatrix);
   drawFace(armface);

   if(matatpos > 55){
      rewindDepthVideo(matat);
      matatpos = 1;
   }
}

void Cube(double time){
   float tm[16], rm[16], mm[16];
   GLuint loc;
   static double prev_time = 0.0;

   translate(tm, 0.5, 0.5, -3.0 - time*1.5);
   rotate(rm, sin(time) / M_PI, cos(time) / M_PI, M_PI);
   matrixMultiply4x4(rm, tm, mm);

   if(time - prev_time > 0.03){
      getNextFrame(matat);
      updateBigCube(matat.buffer, &bigcube);
      matatpos++;
   }

   glUseProgram(bigcube_shader);
   loc = glGetUniformLocation(bigcube_shader, "camera");
   glUniformMatrix4fv(loc, 1, GL_FALSE, mm);
   loc = glGetUniformLocation(bigcube_shader, "pmatrix");
   glUniformMatrix4fv(loc, 1, GL_FALSE, pmatrix);

   drawBigCube(bigcube);

   if(matatpos > 55){
      rewindDepthVideo(matat);
      matatpos = 1;
   }
}

void End(){
   GLuint loc;

   glUseProgram(credits_shader);
   glActiveTexture(GL_TEXTURE0);
   glBindTexture(GL_TEXTURE_2D, atpartytex);
   loc = glGetUniformLocation(credits_shader, "image");
   glUniform1i(loc, 0);
   loc = glGetUniformLocation(credits_shader, "time");
   glUniform1f(loc, 0.0);
   loc = glGetUniformLocation(credits_shader, "distfactor");
   glUniform1f(loc, 0.0);
   loc = glGetUniformLocation(background_shader, "voffset");
   glUniform2f(loc, 0.0, 0.0);

   drawBackground();
}

/*
   *********
   Demo loop
   *********
*/

void Livininabox(){
   double time;
   time = getTimeInterval();
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   if(time >  0.0   && time < 16.17)     KnockKnock(time);
   if(time > 16.04  && time < 27.07)     drawExplosion(time - 16.04);
   if(time > 16.17  && time < 27.07)     JumpOut(time - 16.17);
   if(time > 27.07  && time < 52.23)     RunForLife(time - 27.07);
   if(time > 52.23  && time < 66.50)     Credits(time);
   if(time > 66.50  && time < 118.06)    Greetings(time - 66.50);
   if(time > 118.06 && time < 134.00)    ProceduralScenes(time - 118.06);
   if(time > 134.00 && time < 162.94)    FractalMe(time - 134.00);
   if(time > 162.94 && time < 196.94)    CubeTunnel(time - 162.94);
   if(time > 196.94 && time < 221.26)    Trap(time - 196.94);
   if(time > 221.26 && time < 226.86)    HangingFruite(time - 221.26);
   if(time > 226.86 && time < 248.49)    WatchOutForTheLetters(time - 226.86);
   if(time > 248.49 && time < 271.87)    OpenTheDoor(time - 248.49);
   if(time > 271.87 && time < 276.85)    Headbreak(time);
   if(time > 276.85 && time < 283.33)    Matatas(time);
   if(time > 283.33 && time < 300.00)    Cube(time - 283.33);
   if(time > 300.00 && time < 307.00)    End();

   if(time > 307.00) cg_isrunning = 0;
}

void FreeLivin(){
   int i;

   closeDepthVideo(head);
   closeDepthVideo(matat);

   closeOutline(knock);
   closeOutline(jump);
   closeOutline(run);
   closeOutline(hang);
   closeOutline(falling);
   closeOutline(tiger);
   closeOutline(hop);
   closeOutline(flip);
   closeOutline(run2);
   closeOutline(tbill);
   closeOutline(catwheel);
   closeOutline(katf);
   closeOutline(door);
   closeOutline(trap);
   closeOutline(fractalme);

   for(i = 0; i < LETTERNUM; i++){
      freeMesh3D(letters[i]);
   }

   freeMesh3D(cube);

   deleteBox();
   deleteBackground();
   deleteGreetings();
   freeBigCube(bigcube);

   deleteProgram(background_shader, 5, background_src);
   deleteProgram(title_shader, 5, title_src);
   deleteProgram(credits_shader, 5, credits_src);
   deleteProgram(letter_shader, 2, letter_src);
   deleteProgram(scroll_shader, 5, scroll_src);
   deleteProgram(cube_shader, 3, cube_src);
   deleteProgram(gr_shader, 2, gr_src);
   deleteProgram(bigcube_shader, 2, bigcube_src);

   freeExplosion();
   looseFace(face);
   looseFace(armface);
}
