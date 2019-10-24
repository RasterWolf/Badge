#include "RenderTarget.h"
#include "MyGL.h"
#include "RenderPasses.h"
#include "BadgeEngine.h"

RenderTargetFlipFlop* GRenderTargets;

RenderTarget::RenderTarget(int InSizeX, int InSizeY)
	: SizeX(InSizeX)
	, SizeY(InSizeY)
{
	glGenTextures(1, &ShaderResourceId);
	GL_ASSERT;

	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, ShaderResourceId);
	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SizeX, SizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	GL_ASSERT;

	glGenFramebuffers(1, &RenderTargetId);
	glBindFramebuffer(GL_FRAMEBUFFER, RenderTargetId);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ShaderResourceId, 0);
	GL_ASSERT;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	GL_ASSERT;
}

RenderTarget::~RenderTarget()
{
	glDeleteTextures(1, &ShaderResourceId);
	glDeleteFramebuffers(1, &RenderTargetId);
}

RenderTargetFlipFlop::RenderTargetFlipFlop()
{
	RtIndex = 0;
	SceneTargets[0] = new RenderTarget(GEngine.GetAppWidth(), GEngine.GetAppHeight());
	SceneTargets[1] = new RenderTarget(GEngine.GetAppWidth(), GEngine.GetAppHeight());
}

RenderTargetFlipFlop::~RenderTargetFlipFlop()
{
}

RenderTarget * RenderTargetFlipFlop::GetRenderTarget()
{
	RtIndex ^= 1;
	return SceneTargets[RtIndex];
}
