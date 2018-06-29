#pragma once
#include "StaticGeo.h"
#include "TextureManager.h"
#include "Math.h"

class RenderPasses
{
public:
	void RenderFullScreen(const BadgeImage& Texture);
	void RenderFullScreen(unsigned int Texture);
	void RenderImageBox(const BadgeImage& Texture , const glm::mat4& ObjectMatrix, bool EnableAlpha = false, const glm::vec3& colorMod = glm::vec3(1.0f));
	void InitGL(int* argc, char *argv[]);
	void DestroyGL();
	void CheckForError();
	void SwapBuffers();
};

extern RenderPasses* GRenderPasses;
