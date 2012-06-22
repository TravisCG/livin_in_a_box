typedef struct _BigCube {
   GLuint        vbo[2];
   GLuint        vao;
   GLfloat      *vertices;
   GLuint       *indices;
   unsigned int  indnum;
   unsigned int  vernum;
   int segments;
}BigCube;

BigCube createBigCube(int segments);
void freeBigCube(BigCube bc);
void drawBigCube(BigCube bc);
void updateBigCube(unsigned char *buffer, BigCube *bc);
