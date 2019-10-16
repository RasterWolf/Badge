#include "Platform.h"
#include "MyGL.h"
#include "RenderPasses.h"
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <iostream>


SDL_Window *window;

void checkSDLError(int line = -1)
{
#ifndef NDEBUG
	const char *error = SDL_GetError();
	if (*error != '\0')
	{
		printf("SDL Error: %s\n", error);
		if (line != -1)
			printf(" + line: %i\n", line);
		SDL_ClearError();
	}
#endif
}

void InnerInitSDL(const IVector2D& WindowSize,uint32_t SDLInitFlags, uint32_t SDLWindowFlags, bool bCreateWindow)
{
	SDL_Init(SDLInitFlags);

	if (bCreateWindow)
	{
		window = SDL_CreateWindow(
			"RasterBadge",					   // window title
			SDL_WINDOWPOS_UNDEFINED,           // initial x position
			SDL_WINDOWPOS_UNDEFINED,           // initial y position
			WindowSize.X,                               // width, in pixels
			WindowSize.Y,                               // height, in pixels
			SDLWindowFlags				   // flags - see below
		);
	}
	

	if (SDLWindowFlags & SDL_WINDOW_OPENGL)
	{
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
		SDL_GLContext glcontext = SDL_GL_CreateContext(window);
		SDL_GetError();
		GL_ASSERT;
		SDL_GL_SetSwapInterval(1);
		checkSDLError(__LINE__);
	}
}

#if _MSC_VER

IVector2D Platform::WindowSize(480, 720);

void Platform::Init()
{
	InnerInitSDL(Platform::WindowSize, SDL_INIT_TIMER | SDL_INIT_VIDEO, SDL_WINDOW_OPENGL,true);
}

#elif __SWITCH__

IVector2D Platform::WindowSize;

void Platform::Init()
{
	InnerInitSDL(WindowSize, SDL_INIT_TIMER , 0U,false);
}

#else
IVector2D Platform::WindowSize;

void Platform::Init()
{
	InnerInitSDL(WindowSize, SDL_INIT_TIMER | SDL_INIT_VIDEO, SDL_WINDOW_FULLSCREEN_DESKTOP | SDL_WINDOW_BORDERLESS,true);
}

#endif

IVector2D Platform::GetWindowSize()
{
	return Platform::WindowSize;
}

void Platform::DeInit()
{
	if (window)
	{
		SDL_DestroyWindow(window);
	}
	SDL_Quit();
}

float Platform::GetTimeSeconds()
{
	return SDL_GetTicks() / 1000.0f;
}

void Platform::Delay(uint32_t ms)
{
	SDL_Delay(ms);
}

bool Platform::PollEvent(SDL_Event* event)
{
	return SDL_PollEvent(event);
}

void Platform::SwapBuffers()
{
#if _MSC_VER
	SDL_GL_SwapWindow(window);
#else
	GRenderPasses->SwapBuffers();
#endif
}
