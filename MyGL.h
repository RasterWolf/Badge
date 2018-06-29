#if defined(_MSC_VER)
#define USE_GLEW 1
#define USE_GLUT 1
#include <GL/glew.h>
#include <GL/glut.h>
#else
#include "bcm_host.h"
#include "GLES2/gl2.h"
#include "EGL/egl.h"
#include "EGL/eglext.h"
#endif