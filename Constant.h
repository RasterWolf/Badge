#pragma once

#if defined(_MSC_VER)
#define SCALE 1.5f
#else
#define SCALE 1.0f
#endif
#define SIZE_X ((int)(320 * SCALE))
#define SIZE_Y ((int)(480 * SCALE))
