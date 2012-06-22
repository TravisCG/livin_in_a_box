#include <stdio.h>
#include <GL/gl.h>
#include <locale.h>

#include "cg_x11.h"
#include "cg_music.h"
#include "cg_dialog.h"
#include "cg_constants.h"
#include "livin.h"

int main(int argc, char *argv[]){
   int width = 1024, height = 768, fullscreen = 0;

   if(cgDialog(&width, &height, &fullscreen) == CG_QUIT){
      return(EXIT_SUCCESS);
   }
   setlocale(LC_ALL, "C");
   CreateScene(width, height, fullscreen);
   InitLivin(width, height);

   InitMusic("data/dwain.ogg");

   BEGIN_EVENT
      Livininabox();
      SyncPlay();
   END_EVENT

   FreeLivin();
   FreeMusic();
   RestoreScene();
   return(EXIT_SUCCESS);
}
