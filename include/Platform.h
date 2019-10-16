#pragma once
#include <Vectors.h>
#include <SDL2/SDL_events.h>

class Platform
{
public:
	static void Init();
	static void DeInit();
	static IVector2D GetWindowSize();
	static float GetTimeSeconds();
	static void Delay(unsigned int ms);
	static bool PollEvent(SDL_Event* event);
	static void SwapBuffers(); //I don't like this here. Will probably be changed in the future

private:
	static IVector2D WindowSize;
};