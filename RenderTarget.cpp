#include "RenderTarget.h"
#include "MyGL.h"
#include "RenderPasses.h"
#include "BadgeEngine.h"

RenderTargetFlipFlop* GRenderTargets;

RenderTarget::RenderTarget(int SizeX, int SizeY)
{
	glGenFramebuffers(1, &RenderTargetId);
	glBindFramebuffer(GL_FRAMEBUFFER, RenderTargetId);

	glGenTextures(1, &ShaderResourceId);

	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, ShaderResourceId);

	// Give an empty image to OpenGL ( the last "0" )
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, SizeX, SizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);

	// Poor filtering. Needed !
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ShaderResourceId, 0);

	GRenderPasses->CheckForError();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

RenderTarget::~RenderTarget()
{
	glDeleteTextures(1, &ShaderResourceId);
	glDeleteFramebuffers(1, &RenderTargetId);
}

RenderTargetFlipFlop::RenderTargetFlipFlop()
{
	RtIndex = 0;
	SceneTargets[0] = new RenderTarget(GEngine.GetWidth(), GEngine.GetHeight());
	SceneTargets[1] = new RenderTarget(GEngine.GetWidth(), GEngine.GetHeight());
}

RenderTargetFlipFlop::~RenderTargetFlipFlop()
{
}

RenderTarget * RenderTargetFlipFlop::GetRenderTarget()
{
	RtIndex ^= 1;
	return SceneTargets[RtIndex];
}
