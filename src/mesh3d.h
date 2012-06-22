#define MESHVERTEXINDEX 0
#define MESHNORMALINDEX 1
#define MESHCOLORINDEX  2
#define MESHTEXINDEX    3
#define MESHINDICEINDEX 4
/*
   Mesh3D struct simply store the 3d object
   OpenGL can easily use this
*/
typedef struct Mesh3D{
   GLfloat *vertices;
   GLfloat *normals;
   GLfloat *colors;
   GLfloat *texcoords;
   GLuint  *indices;

   unsigned int      vcount;/* Number of vertices            */
   unsigned int      ncount;/* Number of normals             */
   unsigned int      ccount;/* Number of colors              */
   unsigned int      tcount;/* Number of texture coordinatas */
   unsigned int      icount;/* Number of indices             */

   /* This structure identify the mesh in the OpenGL context */
   GLuint vao;
   GLuint vbo[5];
} Mesh3D;

/* 
   This load OBJ file and store it in Mesh3D struct if 
   something goes wrong, it is returns NULL.
*/
Mesh3D *loadOBJ(const char *filename);

int drawMesh3D(Mesh3D *mesh);
int freeMesh3D(Mesh3D *mesh);

int changeMeshColor(Mesh3D *mesh, float r, float g, float b, float a);
int rndMeshColor(Mesh3D *mesh);
