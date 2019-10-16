#pragma once

//Ortho size not screen size
#define SIZE_X ((int)(320))
#define SIZE_Y ((int)(480))

#if _MSC_VER || __SWITCH__
static const char* AppPath = "./";
#else
const char AppPath[512] = { 0 };
#endif
