#include <iostream>
#include <cstdlib>
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
//#include <GL/glew.h>
//#include <GL/gl.h>
#include <stdlib.h>
#include <time.h>


#include "MyGL.h"

using namespace std;

//My stuff
#include "Shaders.h"
#include "TextureManager.h"
#include "BadgePrograms.h"
#include "Constant.h"
#include "RenderTarget.h"
#include "RenderPasses.h"

extern void InitGL(int* argc, char *argv[]);
extern void StartMainLoop();

BadgeProgram* bb;

void Quit()
{
	TextureManager::DeleteTextureManager();
	ShaderPrograms::DeleteShaderPrograms();
	SDL_Quit();
	exit(0);
}

unsigned int LastRenderTime = 0;
const unsigned int FPS = 60;
const unsigned int FrameTime = 1000/FPS;
const float TickTime = 1000.0f / FPS;

bool Idle()
{
    unsigned int Start  = SDL_GetTicks();
	bool draw = LastRenderTime == 0;
	draw |= bb->Integrate(TickTime);
	if(draw)
	{
		//RenderTarget rt(SIZE_X, SIZE_Y);
		//glBindFramebuffer(GL_FRAMEBUFFER, rt.RenderTargetId);
#if 1
        bb->DoYourThing(TickTime);
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
		//RenderPasses::GetRenderPasses().RenderFullScreen(rt.ShaderResourceId);

		LastRenderTime = Start;

	}

	auto diff = SDL_GetTicks() - Start;

    #ifdef ON_PI
    if(diff > FrameTime)
        std::cout << Start << " " << SDL_GetTicks() << " " << diff << std::endl;
    #endif
	if(diff< FrameTime)
	{
		SDL_Delay(FrameTime - diff);
	}

	return draw;
}

void Render(void)
{
	/*unsigned int Start  = SDL_GetTicks();

	{
        bb->DoYourThing(TickTime);
        glFlush();
	}

	unsigned int diff = SDL_GetTicks() - Start;

	if(diff< FrameTime)
	{
		SDL_Delay(FrameTime - diff);
	}*/
}

void KeyEnter(unsigned char key, int x, int y)
{
  if( key == 'q' ) Quit();
  if (key == 'r')
  {
	  ShaderPrograms::DeleteShaderPrograms();
	  TextureManager::DeleteTextureManager();

	  ShaderPrograms::InitShaderPrograms();
	  TextureManager::InitTextureManager();

  }
  bb->HandleKeyPress(key);
}

void HandleLeftClick(int x, int y)
{
	bb->HandleClick(x,y);
}

int main(int argc, char *argv[])
{
	srand(time(nullptr));
	SDL_Init(SDL_INIT_TIMER);
	InitGL(&argc,argv);
	ShaderPrograms::InitShaderPrograms();
	TextureManager::InitTextureManager();

	RasterBadge rb;
	PulexBadge pb;
	bb = (BadgeProgram*)&pb;

	StartMainLoop();

	return 0;
}
