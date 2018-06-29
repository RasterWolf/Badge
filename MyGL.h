﻿#if defined(_MSC_VER)
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

#if 1
#include <cassert>
#include <iostream>
#define GL_ASSERT {GLenum err = glGetError(); if(err != GL_NO_ERROR){std::cout<<"GL error: " << err << "Function: " << __FUNCTION__ <<std::endl; exit(0);}} 
#else
#define GL_ASSERT
#endif