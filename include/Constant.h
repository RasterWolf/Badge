#pragma once

//Ortho size not screen size
#define SIZE_X ((int)(320))
#define SIZE_Y ((int)(480))

#if _MSC_VER
static const char* AppPath = "./";
#else
static const char* AppPath = "/home/pi/Desktop/badge/";
#endif
