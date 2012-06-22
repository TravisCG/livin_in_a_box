typedef struct _Triangle {
   float x;
   float y;
   struct _Triangle *next;
   struct _Triangle *prev;
}Triangle;

typedef struct _TriangleDList {
   Triangle *tail;
   Triangle *list;
   int count;
}TriangleDList;

void addElement(TriangleDList *triangles, float x, float y);
void removeElement(TriangleDList *triangles, Triangle *del);
void deleteAll(TriangleDList *triangles);
