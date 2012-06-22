#include <stdio.h>
#include <stdlib.h>
#include "tridlist.h"

void addElement(TriangleDList *triangles, float x, float y){
   Triangle *newtriangle;

   newtriangle = malloc(sizeof(Triangle));
   newtriangle->x = x;
   newtriangle->y = y;
   newtriangle->prev = NULL;
   newtriangle->next = triangles->list;
   if(triangles->list != NULL){
      triangles->list->prev = newtriangle;
   }
   else{
      triangles->tail = newtriangle;
   }

   triangles->list = newtriangle;
   triangles->count++;
}

void removeElement(TriangleDList *triangles, Triangle *del){
   Triangle *dummy;

   dummy = del->next;
   del->next->prev = del->prev;
   del->prev->next = dummy;
   free(del);
   triangles->count--;
}

void deleteAll(TriangleDList *triangles){
   Triangle *iter;

   for(iter = triangles->list; iter != NULL; iter = iter->next){
      free(iter);
   }
}
