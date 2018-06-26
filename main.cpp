#include <iostream>

using namespace std;

#include "BadgeEngine.h"
#include "BadgePrograms.h"

int main(int argc, char *argv[])
{
	GEngine.Initialize(&argc,argv);

	RasterBadge rb;
	PulexBadge pb;
	GEngine.SetBadgeProgram(&rb);
	GEngine.MainLoop();
	GEngine.Shutdown();

	return 0;
}
