#pragma once

//Ortho size not screen size
#define SIZE_X ((int)(1280))
#define SIZE_Y ((int)(720))
#define APP_SIZE_Y SIZE_X
#define APP_SIZE_X SIZE_Y

#if _MSC_VER
static const char* AppPath = "./";
#elif __SWITCH__
static const char* AppPath = "romfs:/";
#else
const char AppPath[512] = { 0 };
#endif
