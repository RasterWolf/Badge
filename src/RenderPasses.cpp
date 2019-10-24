#include "RenderPasses.h"
#include "Shaders.h"
#include "StaticGeo.h"

#include "MyGL.h"
#include "glmMath.h"
#include "Constant.h"


RenderPasses* GRenderPasses = nullptr;

RenderPasses::RenderPasses()
{
	matrixStack.emplace(glm::mat4(1.0f));
}

void RenderPasses::PushMatrix(const glm::mat4& mat)
{
	matrixStack.emplace(mat*matrixStack.top());
}

void RenderPasses::PopMatrix()
{
	matrixStack.pop();
}

void RenderPasses::SetRenderTarget(const RenderTarget * rt)
{
	if (rt)
		glBindFramebuffer(GL_FRAMEBUFFER, rt->RenderTargetId);
	else
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RenderPasses::RenderFullScreen(const BadgeImage& Texture)
{
	RenderFullScreen(Texture.TextureId);
}

void RenderPasses::RenderFullScreen(unsigned int Texture)
{
	auto programid = ShaderPrograms::SetProgram(SP_FullScreen);
		
	static GLuint ProjectionMatrixId = glGetUniformLocation(programid, "ObjectMat");
	glUniformMatrix4fv(ProjectionMatrixId, 1, GL_FALSE, glm::value_ptr(matrixStack.top()));


	GL_ASSERT;
	glBindTexture(GL_TEXTURE_2D, Texture);
	GL_ASSERT;
	GUnitCube->Draw();
	GL_ASSERT;
}

void RenderPasses::RenderFullScreenInverted(unsigned int Texture)
{
	ShaderPrograms::SetProgram(SP_FullScreen);
	GL_ASSERT;
	glBindTexture(GL_TEXTURE_2D, Texture);
	GL_ASSERT;
	GUnitCube->Draw(true);
	GL_ASSERT;
}

void RenderPasses::RenderImageBox(const BadgeImage& Texture, bool EnableAlpha, const glm::vec3& colorMod)
{
	auto programid = ShaderPrograms::SetProgram(SP_ImageBox);

	glm::mat4 ViewProjection = glm::ortho(0.0f,(float)SIZE_X,0.0f,(float)SIZE_Y,0.1f,100.0f); //TODO
	ViewProjection = glm::translate(ViewProjection,glm::vec3(0.0f,0.0f,-1.0f));
	//ViewProjection = glm::rotate(ViewProjection, DegToRad(90.0f), glm::vec3(0, 0, 1.0f));

	static GLuint ObjectMatrixId = glGetUniformLocation(programid, "Transform");
	static GLuint ProjectionMatrixId = glGetUniformLocation(programid, "Projection");
	static GLuint colorModId = glGetUniformLocation(programid, "ColorMod");

	glUniformMatrix4fv(ObjectMatrixId, 1, GL_FALSE, glm::value_ptr(matrixStack.top()));
	glUniformMatrix4fv(ProjectionMatrixId, 1, GL_FALSE, glm::value_ptr(ViewProjection));
	glUniform3f(colorModId, colorMod.x, colorMod.y, colorMod.z);

	glBindTexture(GL_TEXTURE_2D, Texture.TextureId);

	if(EnableAlpha)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		GL_ASSERT;
	}

#if 0
	glm::mat4 mvp = ViewProjection * ObjectMatrix;
	glm::vec4 p1 = mvp * glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);
	glm::vec4 p2 = mvp * glm::vec4(1.0f, -1.0f, 0.0f, 1.0f);
	glm::vec4 p3 = mvp * glm::vec4(-1.0f, -1.0f, 0.0f, 1.0f);
	glm::vec4 p4 = mvp * glm::vec4(-1.0f, 1.0f, 0.0f, 1.0f);
#endif 
	GUnitCube->Draw();
	GL_ASSERT;

	if(EnableAlpha)
	{
		glDisable(GL_BLEND);
		GL_ASSERT;

	}

}

void RenderPasses::RenderObj(const ObjModel & model, const BadgeImage& Texture, const glm::mat4 & ObjectMatrix)
{
	auto programid = ShaderPrograms::SetProgram(SP_ImageBox);

	glm::mat4 ViewProjection = glm::ortho(0.0f, (float)SIZE_X, 0.0f, (float)SIZE_Y, 0.1f, 100.0f); //TODO
	ViewProjection = glm::translate(ViewProjection, glm::vec3(0.0f, 0.0f, -1.0f));

	static GLuint ObjectMatrixId = glGetUniformLocation(programid, "Transform");
	static GLuint ProjectionMatrixId = glGetUniformLocation(programid, "Projection");
	static GLuint colorModId = glGetUniformLocation(programid, "ColorMod");

	glUniformMatrix4fv(ObjectMatrixId, 1, GL_FALSE, glm::value_ptr(ObjectMatrix));
	glUniformMatrix4fv(ProjectionMatrixId, 1, GL_FALSE, glm::value_ptr(ViewProjection));
	glUniform3f(colorModId, 1.0f, 1.0f, 1.0f);

	glBindTexture(GL_TEXTURE_2D, Texture.TextureId);
	glDisable(GL_CULL_FACE);
	model.Draw();
	GL_ASSERT;
}

void RenderPasses::CheckForError()
{
	GLenum err = glGetError();
	if (err != GL_NO_ERROR)
	{
		std::cout << "GL Error: " << err << std::endl;
	}
}

#if _MSC_VER
void RenderPasses::InitGL()
{
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
	}

	glDisable(GL_DEPTH_TEST);

	glClearColor(0.0, 0.0, 0.0, 1.0);
	//gluOrtho2D(0, SIZE_X, 0, SIZE_Y);

	std::cout << glGetString(GL_RENDERER) << std::endl;
	std::cout << glGetString(GL_VERSION) << std::endl;

	

}

void RenderPasses::DestroyGL()
{
}
void RenderPasses::SwapBuffers()
{
}
#elif __SWITCH__

#ifndef ENABLE_NXLINK
#define TRACE(fmt,...) ((void)0)
#else
#include <unistd.h>
#define TRACE(fmt,...) printf("%s: " fmt "\n", __PRETTY_FUNCTION__, ## __VA_ARGS__)
#endif


static EGLDisplay s_display;
static EGLContext s_context;
static EGLSurface s_surface;

#define CASE_STR( value ) case value: return #value; 
const char* eglGetErrorString(EGLint error)
{
	switch (error)
	{
		CASE_STR(EGL_SUCCESS)
			CASE_STR(EGL_NOT_INITIALIZED)
			CASE_STR(EGL_BAD_ACCESS)
			CASE_STR(EGL_BAD_ALLOC)
			CASE_STR(EGL_BAD_ATTRIBUTE)
			CASE_STR(EGL_BAD_CONTEXT)
			CASE_STR(EGL_BAD_CONFIG)
			CASE_STR(EGL_BAD_CURRENT_SURFACE)
			CASE_STR(EGL_BAD_DISPLAY)
			CASE_STR(EGL_BAD_SURFACE)
			CASE_STR(EGL_BAD_MATCH)
			CASE_STR(EGL_BAD_PARAMETER)
			CASE_STR(EGL_BAD_NATIVE_PIXMAP)
			CASE_STR(EGL_BAD_NATIVE_WINDOW)
			CASE_STR(EGL_CONTEXT_LOST)
	default: return "Unknown";
	}
}
#undef CASE_STR

void RenderPasses::InitGL()
{
	NWindow* win = nwindowGetDefault();

	// Connect to the EGL default display
	s_display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
	if (!s_display)
	{
		TRACE("Could not connect to display! error: %d", eglGetError());
		goto _fail0;
	}

	// Initialize the EGL display connection
	eglInitialize(s_display, nullptr, nullptr);

	// Select OpenGL (Core) as the desired graphics API
	if (eglBindAPI(EGL_OPENGL_API) == EGL_FALSE)
	{
		TRACE("Could not set API! error: %d", eglGetError());
		goto _fail1;
	}

	// Get an appropriate EGL framebuffer configuration
	EGLConfig config;
	EGLint numConfigs;
	static const EGLint framebufferAttributeList[] =
	{
		EGL_RENDERABLE_TYPE, EGL_OPENGL_BIT,
		EGL_RED_SIZE,     8,
		EGL_GREEN_SIZE,   8,
		EGL_BLUE_SIZE,    8,
		EGL_ALPHA_SIZE,   8,
		EGL_DEPTH_SIZE,   24,
		EGL_STENCIL_SIZE, 8,
		EGL_NONE
	}; 
	eglChooseConfig(s_display, framebufferAttributeList, &config, 1, &numConfigs);
	if (numConfigs == 0)
	{
		TRACE("No config found! error: %d", eglGetError());
		goto _fail1;
	}

	// Create an EGL window surface 
	s_surface = eglCreateWindowSurface(s_display, config, win, nullptr);
	if (!s_surface)
	{
		TRACE("Surface creation failed! error: %d", eglGetError());
		goto _fail1;
	}

	// Create an EGL rendering context
	static const EGLint contextAttributeList[] =
	{
		EGL_CONTEXT_OPENGL_PROFILE_MASK_KHR, EGL_CONTEXT_OPENGL_CORE_PROFILE_BIT_KHR,
		EGL_CONTEXT_MAJOR_VERSION_KHR, 4,
		EGL_CONTEXT_MINOR_VERSION_KHR, 3,
		EGL_NONE
	};
	s_context = eglCreateContext(s_display, config, EGL_NO_CONTEXT, contextAttributeList);
	if (!s_context)
	{
		TRACE("Context creation failed! error: %d", eglGetError());
		goto _fail2;
	}

	// Connect the context to the surface
	eglMakeCurrent(s_display, s_surface, s_surface, s_context);

	gladLoadGL();

	std::cout << glGetString(GL_RENDERER) << std::endl;
	std::cout << glGetString(GL_VERSION) << std::endl;

	return;

_fail2:
	eglDestroySurface(s_display, s_surface);
	s_surface = nullptr;
_fail1:
	eglTerminate(s_display);
	s_display = nullptr;
_fail0:
	return;
}

void RenderPasses::DestroyGL()
{
	if (s_display)
	{
		eglMakeCurrent(s_display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
		if (s_context)
		{
			eglDestroyContext(s_display, s_context);
			s_context = nullptr;
		}
		if (s_surface)
		{
			eglDestroySurface(s_display, s_surface);
			s_surface = nullptr;
		}
		eglTerminate(s_display);
		s_display = nullptr;
	}
}
void RenderPasses::SwapBuffers()
{
	glFlush();
	glFinish();
	eglSwapBuffers(s_display, s_surface);
}
#else //EGL

typedef struct
{
	uint32_t screen_width;
	uint32_t screen_height;
	// OpenGL|ES objects
	DISPMANX_DISPLAY_HANDLE_T dispman_display;
	DISPMANX_ELEMENT_HANDLE_T dispman_element;
	EGLDisplay display;
	EGLSurface surface;
	EGLContext context;
	
} GL_CONTEXT;

GL_CONTEXT glContext;

void RenderPasses::InitGL()
{
	bcm_host_init();
	int32_t success = 0;
	EGLBoolean result;
	EGLint num_config;

	static EGL_DISPMANX_WINDOW_T nativewindow;

	DISPMANX_UPDATE_HANDLE_T dispman_update;
	VC_RECT_T dst_rect;
	VC_RECT_T src_rect;

	static const EGLint attribute_list[] =
	{
		EGL_RED_SIZE, 8,
		EGL_GREEN_SIZE, 8,
		EGL_BLUE_SIZE, 8,
		EGL_ALPHA_SIZE, 8,
		EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
		EGL_NONE
	};

	EGLConfig config;

	// get an EGL display connection
	glContext.display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
	assert(glContext.display != EGL_NO_DISPLAY);

	// initialize the EGL display connection
	result = eglInitialize(glContext.display, NULL, NULL);
	assert(EGL_FALSE != result);

	// get an appropriate EGL frame buffer configuration
	result = eglChooseConfig(glContext.display, attribute_list, &config, 1, &num_config);
	assert(EGL_FALSE != result);

	static const EGLint context_attributes[] =
	{
		EGL_CONTEXT_CLIENT_VERSION, 2,
		EGL_NONE
	};

	// create an EGL rendering context
	glContext.context = eglCreateContext(glContext.display, config, EGL_NO_CONTEXT, context_attributes);
	assert(glContext.context != EGL_NO_CONTEXT);

	// create an EGL window surface
	success = graphics_get_display_size(0 /* LCD */, &glContext.screen_width, &glContext.screen_height);
	assert(success >= 0);

	dst_rect.x = 0;
	dst_rect.y = 0;
	dst_rect.width = glContext.screen_width;
	dst_rect.height = glContext.screen_height;

	src_rect.x = 0;
	src_rect.y = 0;
	src_rect.width = glContext.screen_width << 16;
	src_rect.height = glContext.screen_height << 16;

	glContext.dispman_display = vc_dispmanx_display_open(0 /* LCD */);
	dispman_update = vc_dispmanx_update_start(0);

	glContext.dispman_element = vc_dispmanx_element_add(dispman_update, glContext.dispman_display,
		0/*layer*/, &dst_rect, 0/*src*/,
		&src_rect, DISPMANX_PROTECTION_NONE, 0 /*alpha*/, 0/*clamp*/, DISPMANX_NO_ROTATE /*transform*/);

	nativewindow.element = glContext.dispman_element;
	nativewindow.width = glContext.screen_width;
	nativewindow.height = glContext.screen_height;
	vc_dispmanx_update_submit_sync(dispman_update);

	glContext.surface = eglCreateWindowSurface(glContext.display, config, &nativewindow, NULL);
	assert(glContext.surface != EGL_NO_SURFACE);

	// connect the context to the surface
	result = eglMakeCurrent(glContext.display, glContext.surface, glContext.surface, glContext.context);
	assert(EGL_FALSE != result);

	// Set background color and clear buffers

	glClearColor(0.0, 0.0, 0.0, 1.0);
	glViewport(0, 0, glContext.screen_width, glContext.screen_height);


	std::cout << glGetString(GL_RENDERER) << std::endl;
	std::cout << glGetString(GL_VERSION) << std::endl;
	std::cout << "Screen Resolution " << glContext.screen_width << "x" << glContext.screen_height << std::endl;
}

void RenderPasses::SwapBuffers()
{
	glFlush();
	glFinish();

	eglSwapBuffers(glContext.display, glContext.surface);
}

void RenderPasses::DestroyGL()
{
}
#endif


