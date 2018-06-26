#include "RenderTarget.h"
#include "MyGL.h"
#include "RenderPasses.h"

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

	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, ShaderResourceId, 0);

	GRenderPasses->CheckForError();

}

RenderTarget::~RenderTarget()
{
	glDeleteTextures(1, &ShaderResourceId);
	glDeleteFramebuffers(1, &RenderTargetId);
}
