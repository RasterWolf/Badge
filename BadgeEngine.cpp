#include <iostream>
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <time.h>
#include "BadgeEngine.h"
#include "MyGL.h"
#include "Shaders.h"
#include "TextureManager.h"


BadgeEngine GEngine;

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

void BadgeEngine::Initialize(int* argc, char *argv[])
{
	GRenderPasses = new RenderPasses();
	GRenderPasses->InitGL(argc, argv);

	glutDisplayFunc(gluRender);
	glutIdleFunc(glutIdle);

	//input
	glutKeyboardFunc(KeyEnter);
	glutMouseFunc(MouseClick); //Only care about left click


	ShaderPrograms::InitShaderPrograms();
	TextureManager::InitTextureManager();

	srand(time(nullptr));
	SDL_Init(SDL_INIT_TIMER);

	bIsInitialized = true;
}

void BadgeEngine::Shutdown()
{
	if (bIsInitialized)
	{
		ShaderPrograms::DeleteShaderPrograms();
		TextureManager::DeleteTextureManager();
		SDL_Quit();
		
		GRenderPasses->DestroyGL();
		delete GRenderPasses;

		RunningProgram = nullptr;
		
		bIsInitialized = false;

		exit(0);
	}
	
}

void BadgeEngine::MainLoop()
{
	assert(bIsInitialized);

#if USE_GLEW
	glutMainLoop();
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
		draw = LastRenderTime == 0;
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
			//glBindFramebuffer(GL_FRAMEBUFFER, 0);
			//GRenderPasses->RenderFullScreen(rt.ShaderResourceId);

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

