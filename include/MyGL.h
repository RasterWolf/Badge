#if defined(_MSC_VER)
#define USE_GLEW 1
#include <gl\glew.h>
#include <gl\glu.h>
#elif __SWITCH__
//#include "GLES2/gl2.h"
#include <switch.h>
#include <EGL/egl.h>    // EGL library
#include <EGL/eglext.h> // EGL extensions
#include <glad/glad.h>  // glad library (OpenGL loader)
#else
#include "bcm_host.h"
#include "GLES2/gl2.h"
#include "EGL/egl.h"
#include "EGL/eglext.h"
#endif

#if 1
#include <cassert>
#include <iostream>
#define GL_ASSERT {GLenum err = glGetError(); if(err != GL_NO_ERROR){std::cout<<"GL error: " << err << " Function: " << __FUNCTION__ << "; " << __LINE__ <<std::endl; }}
//void funcAssert();
//#define GL_ASSERT funcAssert();

#else
#define GL_ASSERT
#endif