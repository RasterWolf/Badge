#include <iostream>
#include <cstdlib>
#include <cmath>

#include "BadgeEngine.h"
#include "RasterProgram.h"

int main(int argc, char *argv[])
{
	
	std::cout << "starting engine initialization" << std::endl;
	GEngine.Initialize();
	std::cout << "Engine initialized" << std::endl;
	RasterProgram rp;
	GEngine.SetBadgeProgram(&rp);
	GEngine.MainLoop();
	GEngine.Shutdown();
	
	return 0;
}
