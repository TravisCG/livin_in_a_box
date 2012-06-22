#define GL_GLEXT_PROTOTYPES
#define GLX_GLXEXT_PROTOTYPES
#include "cg_x11.h"
#include "cg_constants.h"
#include <stdlib.h>

char cg_isrunning = 1;
/*XVisualInfo            *cg_visual;*/
XRRScreenConfiguration *cg_screen_conf;
int                     cg_orig_size;
Rotation                cg_rotation;
short                   cg_orig_rate;
int                     cg_screen_mode;
GLXContext              cg_context;


void SetScreenSize(int width, int height, XVisualInfo *visual){
   XRRScreenSize *sizes;
   int snum, rnum, i;
   short *rates;

   cg_screen_conf = XRRGetScreenInfo(cg_display, cg_window);
   cg_orig_size   = XRRConfigCurrentConfiguration(cg_screen_conf, &cg_rotation);
   cg_orig_rate   = XRRConfigCurrentRate(cg_screen_conf);

   /* Get the available resolutions */
   sizes = XRRSizes(cg_display, visual->screen, &snum);
   for(i = 0; i < snum; i++){
      if((sizes[i].width == width) && (sizes[i].height == height)){
         rates = XRRRates(cg_display, visual->screen, i, &rnum);
         XRRSetScreenConfigAndRate(cg_display, cg_screen_conf, 
                                   DefaultRootWindow(cg_display), i, 
                                   cg_rotation, rates[0], CurrentTime);
      }
   }
}

void HideCursor(){
   /* We don't want to see cursors during the demo */
   Cursor hide;
   Pixmap empty;
   static char empty_data[] = {0,0,0,0,0,0,0,0};
   static XColor black = {0,0,0,0,0,0};

   empty = XCreateBitmapFromData(cg_display, cg_window, empty_data, 8, 8);
   hide  = XCreatePixmapCursor(cg_display, empty, empty, &black, &black, 0, 0);
   XDefineCursor(cg_display, cg_window, hide);
}

/* This method is use the old, opengl 2.0 context creation method */
void CreateSceneCompatible(int width, int height, int mode){
   int conf[] = {GLX_DOUBLEBUFFER, True,
                 GLX_RGBA, 
                 GLX_DEPTH_SIZE, 12, 
                 GLX_RED_SIZE, 1, 
                 GLX_BLUE_SIZE, 1, 
                 GLX_GREEN_SIZE, 1, 
                 None};
   XSetWindowAttributes  swa;
   XVisualInfo          *visual;
   Atom                  fullscreen;

   cg_display       = XOpenDisplay(NULL);
   visual        = glXChooseVisual(cg_display, DefaultScreen(cg_display), conf);
   cg_screen_mode   = mode;

   cg_context       = glXCreateContext(cg_display, visual, NULL, True);
   swa.colormap     = XCreateColormap(cg_display, RootWindow(cg_display, visual->screen), visual->visual, AllocNone);
   swa.border_pixel = 0;
   swa.event_mask   = ExposureMask | StructureNotifyMask | 
                      ButtonPressMask | Button1MotionMask | KeyPressMask;
   cg_window        = XCreateWindow(cg_display, RootWindow(cg_display, visual->screen),
                                    0, 0, width, height, 0, visual->depth, InputOutput,
                                    visual->visual, CWBorderPixel | CWColormap | CWEventMask, &swa);

   if(mode == CG_FULLSCREEN){
      /* Set to fullscreen */
      fullscreen = XInternAtom(cg_display, "_NET_WM_STATE_FULLSCREEN", False);
      XChangeProperty(cg_display, cg_window, XInternAtom(cg_display, "_NET_WM_STATE", False), 
                      XA_ATOM, 32, PropModeReplace,
                      (unsigned char *)&fullscreen, 1);

      HideCursor();
      /* Get the current screen size and set the new one */
      SetScreenSize(width, height, visual);
   }

   glXMakeCurrent(cg_display, cg_window, cg_context);
   XMapWindow(cg_display, cg_window);
}

/*
   This method create a new glx based context
   creation method to create opengl 4.1 context
*/
int CreateScene(int width, int height, int mode){
   int conf[] = {GLX_DOUBLEBUFFER, True,
                 GLX_DEPTH_SIZE, 12,
                 GLX_RED_SIZE,    4,
                 GLX_BLUE_SIZE,   4,
                 GLX_GREEN_SIZE,  4,
                 GLX_ALPHA_SIZE,  4,
                 None};

   int gl4attr[] = {GLX_CONTEXT_MAJOR_VERSION_ARB, 4, /* OpenGL major version number */
                    GLX_CONTEXT_MINOR_VERSION_ARB, 1, /* OpenGL minor version number */
                    GLX_CONTEXT_FLAGS_ARB,
                    GLX_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
                    None};

   GLXFBConfig *fbc;
   int fbcount;
   XSetWindowAttributes swa;
   XVisualInfo *visual;
   Atom fullscreen;

   cg_display = XOpenDisplay(NULL);
   fbc = glXChooseFBConfig(cg_display, DefaultScreen(cg_display), conf, &fbcount);
   if(!fbc){
      return(-1);
   }

   visual            = glXGetVisualFromFBConfig(cg_display, fbc[0]);
   if(!visual){
      return(-1);
   }
   swa.background_pixel = 0;
   swa.border_pixel     = 0;
   swa.colormap         = XCreateColormap(cg_display, RootWindow(cg_display, visual->screen),
                                          visual->visual, AllocNone);
   swa.event_mask       = StructureNotifyMask | ExposureMask | KeyPressMask;
   cg_window            = XCreateWindow(cg_display, RootWindow(cg_display, visual->screen),
                                        0, 0, width, height, 0, visual->depth, InputOutput,
                                        visual->visual, CWBorderPixel | CWColormap | CWEventMask, &swa);
   if(!cg_window){
      return(-1);
   }
   cg_context           = glXCreateContextAttribsARB(cg_display, fbc[0], NULL, True, gl4attr);

   if(mode == CG_FULLSCREEN){
      /* Set to fullscreen */
      fullscreen = XInternAtom(cg_display, "_NET_WM_STATE_FULLSCREEN", False);
      XChangeProperty(cg_display, cg_window, XInternAtom(cg_display, "_NET_WM_STATE", False), 
                      XA_ATOM, 32, PropModeReplace,
                      (unsigned char *)&fullscreen, 1);
      HideCursor();
      /* Get the current screen size and set the new one */
      SetScreenSize(width, height, visual);
   }

   cg_screen_mode = mode;
   XMapWindow(cg_display, cg_window);
   glXMakeCurrent(cg_display, cg_window, cg_context);

   free(fbc);
   XFree(visual);
   return(0);
}

void RestoreScene(){
   /* Restore screen resolution */
   if(cg_screen_mode == CG_FULLSCREEN){
      XRRSetScreenConfigAndRate(cg_display, cg_screen_conf, 
                                DefaultRootWindow(cg_display), cg_orig_size,
                                cg_rotation, cg_orig_rate, CurrentTime);
      XRRFreeScreenConfigInfo(cg_screen_conf);
   }

   /* Free resources */
   glXMakeCurrent(cg_display, 0, 0);
   glXDestroyContext(cg_display, cg_context);
   XDestroyWindow(cg_display, cg_window);
   XCloseDisplay(cg_display);
}
