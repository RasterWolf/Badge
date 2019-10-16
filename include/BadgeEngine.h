#pragma once
#include <cassert>
#include "BadgePrograms.h"
#include "RenderPasses.h"
#include <Vectors.h>


/// BadgeEngine takes the programs and runs them!
/// It handles the input and rendering main loops
class BadgeEngine
{
public:
	BadgeEngine() = default;
	~BadgeEngine() = default;

	void Initialize();
	void Shutdown();
	void SetBadgeProgram(BadgeProgram* prog)
	{
		RunningProgram = prog;
	}

	void MainLoop();
	void HandleLeftClick(float x, float y);
	void HandleKeyPress(unsigned char key, int x, int y);
	int GetWidth() const { return WindowSize.X;  }
	int GetHeight() const { return WindowSize.Y;  }
	const char* GetPath() const;

	bool InnerMainLoop(bool bForceDraw);

	const unsigned int FPS = 60;
	const unsigned int FrameTime = 1000 / FPS;
	const float TickTime = 1000.0f / FPS;
private:
	bool bIsInitialized = false;
	bool bIsExiting = false;
	BadgeProgram* RunningProgram = nullptr;
	float LastRenderTime = 0;
	float LastBatteryCheck = 0;
	IVector2D WindowSize;

};

extern BadgeEngine GEngine;