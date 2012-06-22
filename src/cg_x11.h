#include <GL/glx.h>
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/keysym.h>
#include <X11/extensions/Xrandr.h>

#define BEGIN_EVENT \
while(cg_isrunning){\
   while(XPending(cg_display) > 0){\
      XNextEvent(cg_display, &cg_event);\
      if(cg_event.type == Expose) break;\
      if(cg_event.type == KeyPress){\
         cg_ks = XLookupKeysym((XKeyEvent*)&cg_event, 0);\
         if(cg_ks == XK_Escape) cg_isrunning = 0;\
      }\
   }

#define END_EVENT glXSwapBuffers(cg_display, cg_window);}


/* Global variables for better coding style.
   We use cg_ prefix to identify these.  */

Display                *cg_display;
Window                  cg_window;
XEvent                  cg_event;
KeySym                  cg_ks;
extern char             cg_isrunning;

int CreateScene(int width, int height, int mode);
void RestoreScene();
