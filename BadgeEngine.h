#pragma once
#include <cassert>
#include "BadgePrograms.h"
#include "RenderPasses.h"


/// BadgeEngine takes the programs and runs them!
/// It handles the input and rendering main loops
class BadgeEngine
{
public:
	BadgeEngine() = default;
	~BadgeEngine() = default;

	void Initialize(int* argc, char *argv[]);
	void Shutdown();
	void SetBadgeProgram(BadgeProgram* prog)
	{
		RunningProgram = prog;
	}

	void MainLoop();
	void HandleLeftClick(int x, int y);
	void HandleKeyPress(unsigned char key, int x, int y);

	bool InnerMainLoop(bool bForceDraw);

	const unsigned int FPS = 60;
	const unsigned int FrameTime = 1000 / FPS;
	const float TickTime = 1000.0f / FPS;
private:
	bool bIsInitialized = false;
	BadgeProgram* RunningProgram = nullptr;
	unsigned int LastRenderTime = 0;

};

extern BadgeEngine GEngine;