#include "Platform.h"
#include "MyGL.h"
#include "RenderPasses.h"
#include "Constant.h"
#include <SDL2/SDL.h>
#include <iostream>

Platform GPlatform;

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

#if _MSC_VER

void WindowsPlatform::SwapBuffers()
{
	SDL_GL_SwapWindow(window);
}

#elif __SWITCH__

#include <switch.h>

void SwitchPlatform::Init()
{
	PlatformBase::Init();

	romfsInit();

	if (R_FAILED(socketInitializeDefault()))
		return;

	s_nxlinkSock = nxlinkStdio();
	if (s_nxlinkSock >= 0)
		printf("printf output now goes to nxlink server");
	else
		socketExit();
}

void SwitchPlatform::DeInit()
{
	PlatformBase::DeInit();
	romfsExit();
	if (s_nxlinkSock >= 0)
	{
		close(s_nxlinkSock);
		socketExit();
		s_nxlinkSock = -1;
	}
}

void SwitchPlatform::ScanInput()
{
	PlatformBase::ScanInput();

	//Scan all the inputs. This should be done once for each frame
	hidScanInput();

	//hidKeysDown returns information about which buttons have been just pressed (and they weren't in the previous frame)
	uint64_t kDown = hidKeysDown(CONTROLLER_HANDHELD);

	bRequestExit |= !!(kDown & KEY_PLUS);

	if (kDown&KEY_TOUCH)
	{

		uint32_t touch_count = hidTouchCount();

		glm::vec2 window(WindowSize.X, WindowSize.Y);

		glm::mat4 rot = glm::rotate(glm::mat4(1.0f), DegToRad(-90.0f), glm::vec3(0, 0, 1));
		glm::mat4 tran = glm::translate(glm::mat4(1.0f), glm::vec3(-0.5f, -0.5f, 0));
		glm::mat4 mat = glm::inverse(tran) * rot * tran;

		for (uint32_t i = 0U; i < touch_count; i++)
		{
			touchPosition touch;
			//Read the touch screen coordinates
			hidTouchRead(&touch, i);
			glm::vec2 normalizedTouch = glm::vec2(touch.px, touch.py) / window;

			glm::vec4 mouse = mat * glm::vec4(normalizedTouch.x, normalizedTouch.y, 0, 1.0f);

			SDL_Event event;
			event.type = SDL_FINGERDOWN;
			event.tfinger.pressure = 0.2;
			event.tfinger.x = mouse.x * window.x;
			event.tfinger.y = mouse.y * window.y;
			touchEvents.push(event);

#if 0
			printf("%f x %f; id: %d\n", mouse.x, mouse.y, touch.id);
#endif
		}
	}
}

#else
void RpiPlatform::Init()
{
	PlatformBase::Init();

	char* path = const_cast<char*>(&AppPath[0]);
	getcwd(path, 512);
	std::cout << path << std::endl;

}
#endif


void PlatformBase::Init()
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
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_CreateContext(window);
		SDL_GetError();
		
		GL_ASSERT;
		SDL_GL_SetSwapInterval(1);
		checkSDLError(__LINE__);
	}
}

void PlatformBase::DeInit()
{
	if (window)
	{
		SDL_DestroyWindow(window);
	}
	SDL_Quit();
}

IVector2D PlatformBase::GetWindowSize() const 
{
	return PlatformBase::WindowSize;
}

IVector2D PlatformBase::GetAppSize() const
{
	return AppSize;
}

float PlatformBase::GetTimeSeconds() const
{
	return SDL_GetTicks() / 1000.0f;
}

void PlatformBase::Delay(uint32_t ms) const
{
	SDL_Delay(ms);
}

bool PlatformBase::PollEvent(SDL_Event* event)
{
	return SDL_PollEvent(event);
}

void PlatformBase::SwapBuffers()
{
	GRenderPasses->SwapBuffers();
}