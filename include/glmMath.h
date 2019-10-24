#pragma once
//matrix math (don't want to re-write this
#if !defined(_MSC_VER)
#undef countof
#endif
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define BADGE_PI 3.14159265358979323846

inline float DegToRad(float degrees)
{
	return (degrees * BADGE_PI) / 180.0f;
}