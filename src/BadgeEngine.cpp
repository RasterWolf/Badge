#include <iostream>
#include "Platform.h"
#include <time.h>
#include "BadgeEngine.h"
#include "MyGL.h"
#include "Shaders.h"
#include "TextureManager.h"
#include "Constant.h"
#include "RenderTarget.h"
#include "GpioControls.h"
#if !defined(_MSC_VER)
#include <unistd.h>
#endif

//void funcAssert()
//{
//	GLenum err = glGetError();
//	if (err != GL_NO_ERROR)
//	{
//		std::cout << "GL error: " << err << " Function: " << __FUNCTION__ << std::endl; 
//		
//		__debugbreak();
//		exit(0);
//	}
//}

BadgeEngine GEngine;

void BadgeEngine::Initialize()
{
	srand((unsigned int)time(nullptr));

	GPlatform.Init();
	IVector2D WindowSize = GPlatform.GetWindowSize();

	//Init Engine globals
	GRenderPasses = new RenderPasses();
	GRenderPasses->InitGL();

	glViewport(0, 0, WindowSize.X, WindowSize.Y);

	std::cout << "Loading shaders..." << std::endl;
	ShaderPrograms::InitShaderPrograms();
	std::cout << "Shaders Loaded." << std::endl;

	GL_ASSERT;

	std::cout << "Loading Textures..." << std::endl;
	TextureManager::InitTextureManager();
	std::cout << "Textures Loaded" << std::endl;
	GL_ASSERT;

	GUnitCube = new UnitCubeGeo();
	GL_ASSERT;

	GRenderTargets = new RenderTargetFlipFlop();
	GL_ASSERT;

	GpioControls::InitGpioControls();

	bIsInitialized = true;
	std::cout << "WindowSize: " << WindowSize.X << "x" << WindowSize.Y << std::endl;
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
		
		bIsExiting = true;

		GPlatform.DeInit();
	}
	
}

void BadgeEngine::MainLoop()
{
	assert(bIsInitialized);

	while (!bIsExiting && !GPlatform.PlatformRequestingExit())
	{
		GPlatform.ScanInput(); 

		SDL_Event event;

		while (GPlatform.PollEvent(&event))
		{
			if (event.type == SDL_MOUSEBUTTONUP)
			{
				float fx = event.button.x / (float)GetWidth();
				float fy = event.button.y / (float)GetHeight();
				std::cout << "mouse clicked: " << fx << "x" << fy << std::endl;
				HandleLeftClick(fx, fy);
				//HandleLeftClick(0.14f, 0.46f);
			}

			if (event.type == SDL_FINGERDOWN)
			{
				if (event.tfinger.pressure >= 0.1)
				{
					float fx = event.tfinger.x / (float)GetWidth();
					float fy = event.tfinger.y / (float)GetHeight();
					std::cout << "mouse clicked: " << fx << "x" << fy << std::endl;
					HandleLeftClick(fx, fy);
				}

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
			GPlatform.SwapBuffers();
			//std::cout << "draw" << std::endl;
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

bool BadgeEngine::InnerMainLoop(bool bForceDraw)
{
	float Start = GPlatform.GetTimeSeconds();
	bool draw = bForceDraw;

	bool bCheckBattery = LastBatteryCheck == 0 || ((Start - LastBatteryCheck) > 60);
	if (bCheckBattery)
	{
		if (GpioControls::CheckLowBattery())
		{
			GpioControls::Shutdown();
		}
		LastBatteryCheck = Start;
	}
	
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

	float end = GPlatform.GetTimeSeconds();
	auto diff = (end - Start)*1000.0f; //diff in MS

#if 0
	if (diff > FrameTime)
		std::cout << Start << " " << end << " " << diff << std::endl;
#endif
	if (diff < TickTime)
	{
		GPlatform.Delay(uint32_t(TickTime - diff));
	}

	return draw;
}

