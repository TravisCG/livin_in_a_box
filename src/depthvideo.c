#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "depthvideo.h"

DepthVideo openDepthVideo(char *filename){
   DepthVideo dv;

   dv.videofile = fopen(filename, "r");
   dv.buffer = malloc(sizeof(char) * 640 * 480);
   if(dv.buffer != NULL){
      memset(dv.buffer, 0, sizeof(char) * 640 * 480);
   }
   return(dv);
}

void closeDepthVideo(DepthVideo dv){
  free(dv.buffer);
  fclose(dv.videofile);
}

void rewindDepthVideo(DepthVideo dv){
  rewind(dv.videofile);
  memset(dv.buffer, 0, sizeof(char) * 640 * 480);
}

void getNextFrame(DepthVideo dv){
   unsigned char sizechunk[3], *encoded_data;
   unsigned int read, i, pos;

   fread(sizechunk, sizeof(char), 3, dv.videofile);
   read = 65536u * sizechunk[0] + 256u * sizechunk[1] + sizechunk[2];

   encoded_data = malloc(sizeof(char) * read);
   if(encoded_data == NULL){ printf("Oh my God!\n"); return; } /* Not nice, but safe */

   fread(encoded_data, sizeof(char), read, dv.videofile);

   for(i = 0; i < read / 4; i++){
      pos = encoded_data[i*4] * 65536u + encoded_data[i*4+1] * 256u + encoded_data[i*4+2];
      if(pos > 640 * 480){
         printf("Invalid position in depth video\n");
         continue;
      }
      dv.buffer[pos] = encoded_data[i*4+3];
   }

   free(encoded_data);
}

