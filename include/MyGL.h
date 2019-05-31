#if defined(_MSC_VER)
#define USE_GLEW 1
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <gl\glew.h>
#include <SDL2/SDL_opengl.h>
#include <gl\glu.h>
#else
#include "bcm_host.h"
#include "GLES2/gl2.h"
#include "EGL/egl.h"
#include "EGL/eglext.h"
#endif

#if 0
#include <cassert>
#include <iostream>
#define GL_ASSERT {GLenum err = glGetError(); if(err != GL_NO_ERROR){std::cout<<"GL error: " << err << " Function: " << __FUNCTION__ <<std::endl; exit(0);}}
//void funcAssert();
//#define GL_ASSERT funcAssert();

#else
#define GL_ASSERT
#endif