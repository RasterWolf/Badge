#include <iostream>
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <time.h>
#include "BadgeEngine.h"
#include "MyGL.h"
#include "Shaders.h"
#include "TextureManager.h"
#include "Constant.h"


BadgeEngine GEngine;
#if USE_GLUT
void MouseClick(int button, int state, int x, int y)
{
	std::cout << button << " " << state << " " << x << " " << y << std::endl;
	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
		GEngine.HandleLeftClick(x, y);
}

void glutIdle()
{
	if (GEngine.InnerMainLoop(false))
	{
		glutSwapBuffers();
		GLenum err = glGetError();
		if (err != GL_NO_ERROR)
		{
			std::cout << "GL Error: " << gluErrorString(err) << std::endl;
		}
		glutPostRedisplay();
	}
}

void gluRender()
{
	/*if (GEngine.InnerMainLoop(true))
	{
		glutSwapBuffers();
		GLenum err = glGetError();
		if (err != GL_NO_ERROR)
		{
			std::cout << "GL Error: " << gluErrorString(err) << std::endl;
		}
		glutPostRedisplay();
	}*/
}

void KeyEnter(unsigned char key, int x, int y)
{
	GEngine.HandleKeyPress(key,x,y);
}
#else

SDL_Window *window;
int width;
int height;

#endif

void BadgeEngine::Initialize(int* argc, char *argv[])
{
	GRenderPasses = new RenderPasses();
	GRenderPasses->InitGL(argc, argv);

#if USE_GLUT
	glutDisplayFunc(gluRender);
	glutIdleFunc(glutIdle);

	//input
	glutKeyboardFunc(KeyEnter);
	glutMouseFunc(MouseClick); //Only care about left click
#endif

	ShaderPrograms::InitShaderPrograms();
	TextureManager::InitTextureManager();
	GUnitCube = new UnitCubeGeo();

	srand(time(nullptr));

#if !USE_GLUT
	SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO);

	window = SDL_CreateWindow(
		"RasterBadge",					   // window title
		SDL_WINDOWPOS_UNDEFINED,           // initial x position
		SDL_WINDOWPOS_UNDEFINED,           // initial y position
		0,                               // width, in pixels
		0,                               // height, in pixels
		SDL_WINDOW_FULLSCREEN_DESKTOP| SDL_WINDOW_BORDERLESS                 // flags - see below
	);

	SDL_GetWindowSize(window, &width, &height);
	
#else
	SDL_Init(SDL_INIT_TIMER);
#endif

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
#if !USE_GLUT
		SDL_DestroyWindow(window);
#endif
		SDL_Quit();

		exit(0);
	}
	
}

void BadgeEngine::MainLoop()
{
	assert(bIsInitialized);

#if USE_GLUT
	glutMainLoop();
#else
	while (true)
	{
		SDL_Event event;

		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_MOUSEBUTTONUP)
			{
				float fx = event.button.x / (float)width;
				float fy = event.button.y / (float)height;
				int x = int(fx*SIZE_X);
				int y = int(fy*SIZE_Y);
				std::cout << "mouse clicked: " << x << "x" << y << std::endl;
				HandleLeftClick(x, y);
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
			GRenderPasses->SwapBuffers();
		}
	}
#endif
}

void BadgeEngine::HandleLeftClick(int x, int y)
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

		ShaderPrograms::InitShaderPrograms();
		TextureManager::InitTextureManager();
		new(GUnitCube) UnitCubeGeo();

	}
	if (RunningProgram)
	{
		RunningProgram->HandleKeyPress(key);
	}
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

	auto diff = SDL_GetTicks() - Start;

#ifdef ON_PI
	if (diff > FrameTime)
		std::cout << Start << " " << SDL_GetTicks() << " " << diff << std::endl;
#endif
	if (diff < FrameTime)
	{
		SDL_Delay(FrameTime - diff);
	}

	return draw;
}

