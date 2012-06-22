#define MAX_OUTLINE_POINTS 6000
#define MAX_OUTLINE_INDEX 3000
#define OUTLINE_FPS 0.077

typedef struct _Contours {
   GLfloat *coord;
   int coordnum;
}Contours;

typedef struct _Frames {
   Contours *contours;
   int contournum;
}Frames;

typedef struct _Outline {
   Frames *frames;
   int framenum;
   GLuint vbo[2];
   GLuint vao;
   GLuint shader_src[2];
   GLuint shader;
}Outline;

Outline openOutline(char *filename, char *vssrc, char *fssrc);

void closeOutline(Outline outline);

void drawOutlineFrame(Outline outline, int frame);

void averageCoords(Outline outline, int frame, GLfloat *avg);

/*void interpolateOutlines(Outline from, Outline to, int fromf, int tof, int fromc, int toc, float percent);*/

void drawTriangulatedOutline(Outline outline, int frame);
void interpolateTriangulatedOutlines(Outline from, Outline to, int fromf, int tof, int fromc, int toc, float percent);
