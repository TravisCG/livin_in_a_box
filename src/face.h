typedef struct {
   GLuint vbo[2];
   GLuint vao;
   GLuint shader_src[2];
   GLuint shader;
   GLfloat *verts;
   GLuint *indices;
}Face;

Face *initFace(char *vssrc, char *fssrc);
void looseFace(Face *face);
void updateFace(unsigned char *buffer, Face *face);
void drawFace(Face *face);
