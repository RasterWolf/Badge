#include <iostream>

using namespace std;

#include "BadgeEngine.h"
#include "BadgePrograms.h"

int main(int argc, char *argv[])
{
	GEngine.Initialize();

	RasterProgram rp;
	GEngine.SetBadgeProgram(&rp);
	GEngine.MainLoop();
	GEngine.Shutdown();
	return 0;
}
