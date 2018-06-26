#pragma once
#if defined(_MSC_VER)
#define USE_GLEW 1
#else
#define USE_GLEW 0
#endif

#if USE_GLEW
#include <GL/glew.h>
#include <GL/glut.h>
#else
#include <GLES2/gl2.h>
#endif

extern void GlCheckError();