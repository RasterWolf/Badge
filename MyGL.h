#if defined(_MSC_VER)
#define USE_GLEW 1
#include <GL/glew.h>
#include <GL/glut.h>
#else
#define USE_GLEW 0
#include <GLES2/gl2.h>
#endif