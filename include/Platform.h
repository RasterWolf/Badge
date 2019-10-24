#pragma once
#include <Vectors.h>
#include "Constant.h"
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>

class PlatformBase
{
public:
	void Init();
	void DeInit();
	IVector2D GetWindowSize() const;
	IVector2D GetAppSize() const;
	float GetTimeSeconds() const;
	void Delay(unsigned int ms) const;
	bool PollEvent(SDL_Event* event);
	void ScanInput() { ; }
	void SwapBuffers(); //I don't like this here. Will probably be changed in the future
	bool PlatformRequestingExit() const
	{
		return bRequestExit;
	}

protected:
	PlatformBase(uint32_t InSDLInitFlags, SDL_WindowFlags InSDLWindoFlags, bool InCreateWindow)
		: WindowSize(IVector2D(SIZE_X,SIZE_Y))
		, AppSize(IVector2D(APP_SIZE_X,APP_SIZE_Y))
		, SDLInitFlags(InSDLInitFlags)
		, SDLWindowFlags(InSDLWindoFlags)
		, bCreateWindow(InCreateWindow)
		, window(nullptr)
		, bRequestExit(false)
	{
		;
	}

	const IVector2D WindowSize;
	const IVector2D AppSize;
	const uint32_t SDLInitFlags;
	const SDL_WindowFlags SDLWindowFlags = SDL_WINDOW_OPENGL;
	const bool bCreateWindow;
	SDL_Window *window;
	bool bRequestExit;
};

#if _MSC_VER
class WindowsPlatform : public PlatformBase
{
public:
	void SwapBuffers();
	WindowsPlatform()
		: PlatformBase(SDL_INIT_TIMER|SDL_INIT_VIDEO,SDL_WINDOW_OPENGL,true)
	{
		;
	}
};

typedef WindowsPlatform Platform;

#elif __SWITCH__
#include <queue>
class SwitchPlatform : public PlatformBase
{
public:
	SwitchPlatform()
		: PlatformBase(SDL_INIT_TIMER, SDL_WindowFlags(0U), false)
		, s_nxlinkSock(-1)
	{
		;
	}

	void Init();
	void DeInit();
	void ScanInput();
	bool PollEvent(SDL_Event* event)
	{
		if (!touchEvents.empty())
		{
			*event = touchEvents.front();
			touchEvents.pop();
			return true;
		}

		return PlatformBase::PollEvent(event);
	}

private:
	int32_t s_nxlinkSock;
	std::queue<SDL_Event> touchEvents;
	
};

typedef SwitchPlatform Platform;

#else
class RpiPlatform : public PlatformBase
{
public:
	RpiPlatform()
		: PlatformBase(SDL_INIT_TIMER|SDL_INIT_VIDEO, SDL_WINDOW_FULLSCREEN_DESKTOP | SDL_WINDOW_BORDERLESS, true)
	{
		;
	}
	static void Init();
};
#endif

extern Platform GPlatform;
