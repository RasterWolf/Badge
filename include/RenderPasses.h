#pragma once
#include "StaticGeo.h"
#include "TextureManager.h"
#include "glmMath.h"
#include "RenderTarget.h"

class RenderPasses
{
public:
	void SetRenderTarget(const RenderTarget* rt);
	void RenderFullScreen(const BadgeImage& Texture);
	void RenderFullScreen(unsigned int Texture);
	void RenderFullScreenInverted(unsigned int Texture);
	void RenderImageBox(const BadgeImage& Texture , const glm::mat4& ObjectMatrix, bool EnableAlpha = false, const glm::vec3& colorMod = glm::vec3(1.0f));
	void InitGL();
	void DestroyGL();
	void CheckForError();
	void SwapBuffers();
};

extern RenderPasses* GRenderPasses;
