#include <iostream>
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <time.h>
#include "BadgeEngine.h"
#include "MyGL.h"
#include "Shaders.h"
#include "TextureManager.h"
#include "Constant.h"
#include "RenderTarget.h"

BadgeEngine GEngine;
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

void BadgeEngine::Initialize()
{
	srand((unsigned int)time(nullptr));

	SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO);

#if _MSC_VER
	SDL_WindowFlags flags = SDL_WINDOW_OPENGL;
	const glm::ivec2 WindowSize(480, 720);
#else
	unsigned int flags = SDL_WINDOW_FULLSCREEN_DESKTOP | SDL_WINDOW_BORDERLESS;
	const glm::ivec2 WindowSize(0, 0);
#endif

	window = SDL_CreateWindow(
		"RasterBadge",					   // window title
		SDL_WINDOWPOS_UNDEFINED,           // initial x position
		SDL_WINDOWPOS_UNDEFINED,           // initial y position
		WindowSize.x,                               // width, in pixels
		WindowSize.y,                               // height, in pixels
		flags                // flags - see below
	);

#if _MSC_VER
	SDL_GLContext glcontext = SDL_GL_CreateContext(window);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

	SDL_GL_SetSwapInterval(1);
	checkSDLError(__LINE__);
#endif

	SDL_GetWindowSize(window, &WindowWidth, &WindowHeight);

	GRenderPasses = new RenderPasses();
	GRenderPasses->InitGL();

	glViewport(0, 0, WindowWidth, WindowHeight);


	ShaderPrograms::InitShaderPrograms();
	TextureManager::InitTextureManager();
	GUnitCube = new UnitCubeGeo();
	GRenderTargets = new RenderTargetFlipFlop();


	bIsInitialized = true;
}

void BadgeEngine::Shutdown()
{
	if (bIsInitialized)
	{
		ShaderPrograms::DeleteShaderPrograms();
		TextureManager::DeleteTextureManager();
		delete GUnitCube;
		
		GRenderPasses->DestroyGL();
		delete GRenderPasses;

		RunningProgram = nullptr;
		
		bIsInitialized = false;
		SDL_DestroyWindow(window);
		SDL_Quit();

		exit(0);
	}
	
}

void BadgeEngine::MainLoop()
{
	assert(bIsInitialized);

	while (true)
	{
		SDL_Event event;

		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_MOUSEBUTTONUP)
			{
				float fx = event.button.x / (float)WindowWidth;
				float fy = event.button.y / (float)WindowHeight;
				std::cout << "mouse clicked: " << fx << "x" << fy << std::endl;
				HandleLeftClick(fx, fy);
				//HandleLeftClick(0.14f, 0.46f);
			}

			if (event.type == SDL_KEYDOWN && event.key.state == SDL_PRESSED && event.key.repeat == 0)
			{
				unsigned char c = tolower(SDL_GetKeyName(event.key.keysym.sym)[0]);
				std::cout << "pressing key: " << c << std::endl;
				HandleKeyPress(c, 0, 0);
			}

			if (event.type == SDL_QUIT)
			{
				return;
			}
		
		}

		if (!bIsInitialized)
		{
			return;
		}

		if (InnerMainLoop(false))
		{
#if _MSC_VER
			SDL_GL_SwapWindow(window);
#else
			GRenderPasses->SwapBuffers();
#endif
			std::cout << "draw" << std::endl;
		}
	}
}

void BadgeEngine::HandleLeftClick(float x, float y)
{
	if (RunningProgram)
	{
		RunningProgram->HandleClick(x, y);
	}
}

void BadgeEngine::HandleKeyPress(unsigned char key, int x, int y)
{
	if (key == 'q') GEngine.Shutdown();
	if (key == 'r')
	{
		ShaderPrograms::DeleteShaderPrograms();
		TextureManager::DeleteTextureManager();
		delete GUnitCube;
		delete GRenderTargets;

		ShaderPrograms::InitShaderPrograms();
		TextureManager::InitTextureManager();
		GUnitCube = new UnitCubeGeo();
		GRenderTargets = new RenderTargetFlipFlop();

	}
	if (RunningProgram)
	{
		RunningProgram->HandleKeyPress(key);
	}
}

float BadgeEngine::GetTimeSeconds() const
{
	return SDL_GetTicks() / 1000.0f;
}

bool BadgeEngine::InnerMainLoop(bool bForceDraw)
{
	unsigned int Start = SDL_GetTicks();
	bool draw = bForceDraw;
	
	if (RunningProgram)
	{
		draw |= LastRenderTime == 0 || ((Start - LastRenderTime) > (2 * 60 * 1000)); //Refresh every 2 min (this should keep the screen on)
		draw |= RunningProgram->Integrate(TickTime);
		if (draw)
		{
#if 1
			glClear(GL_COLOR_BUFFER_BIT);
			RunningProgram->Render(TickTime);
			glFlush();
#else
			static int i = 0;
			int c = i % 3;
			if (c == 0)
				glClearColor(1, 0, 0, 1);
			else if (c == 1)
				glClearColor(0, 1, 0, 1);
			else
				glClearColor(0, 0, 1, 1);

			i++;
			
			glClear(GL_COLOR_BUFFER_BIT);
#endif
			LastRenderTime = Start;
		}

	}
	uint32_t end = SDL_GetTicks();
	auto diff = end - Start;

#if 1
	if (diff > FrameTime)
		std::cout << Start << " " << end << " " << diff << std::endl;
#endif
	if (diff < FrameTime)
	{
		std::cout << "sleeping " << (end - Start) <<  std::endl;
		SDL_Delay(FrameTime - diff);
	}

	return draw;
}

