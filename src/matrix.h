void matrixMultiply4x4(float *a, float *b, float *c);

void perspectiveMatrix(float *m, float fov, float aspect, float near, float far);

void translate(float *m, float x, float y, float z);
void rotate(float *m, float rx, float ry, float rz);
void lookAt(float *m, float *eye, float *target, float *up);
