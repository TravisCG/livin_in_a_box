#define PRG_ERROR 1
#define SHADER_ERROR 2

GLuint createShader(GLenum shadertype, const char *source);

GLuint loadShader(GLenum shadertype, const char *filename);

GLuint createProgram(int count, GLuint *shaders);

void deleteProgram(GLuint prg, int count, GLuint *shaders);

void shaderError(GLuint obj, int type);

void bindVarToBuff(GLuint prg, char *varname, GLuint buffer, GLint size);
