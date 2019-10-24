#pragma once
#include "StaticGeo.h"
#include "TextureManager.h"
#include "glmMath.h"
#include "RenderTarget.h"
#include "ObjModel.h"
#include <stack>



class RenderPasses
{
public:
	RenderPasses();
	void SetRenderTarget(const RenderTarget* rt);
	void RenderFullScreen(const BadgeImage& Texture);
	void RenderFullScreen(unsigned int Texture);
	void RenderFullScreenInverted(unsigned int Texture);
	void RenderImageBox(const BadgeImage& Texture , bool EnableAlpha = false, const glm::vec3& colorMod = glm::vec3(1.0f));
	void RenderObj(const ObjModel& model, const BadgeImage& Texture, const glm::mat4& ObjectMatrix);
	void InitGL();
	void DestroyGL();
	void CheckForError();
	void SwapBuffers();
	void PushMatrix(const glm::mat4& mat);
	void PopMatrix();
	const glm::mat4& MatrixStackTop() const
	{
		return matrixStack.top();
	}
	void PushIdentity()
	{
		matrixStack.emplace(glm::mat4(1.0f));
	}
protected:
	std::stack<glm::mat4> matrixStack;
};

extern RenderPasses* GRenderPasses;

class ScopeApplyTransform
{
public:
	ScopeApplyTransform(const glm::mat4& mat)
	{
		GRenderPasses->PushMatrix(mat);
	}

	~ScopeApplyTransform()
	{
		GRenderPasses->PopMatrix();
	}
};

#define SCOPE_TRANSFORM(x) ScopeApplyTransform sat##__LINE__((x))
