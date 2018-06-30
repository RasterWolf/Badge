#include "PostEffects.h"
#include "RenderPasses.h"

void BasePostEffect::SetProgram(BadgeProgram * prog)
{
	RunningProgram = prog;
}

void BasePostEffect::HandleClick(float x, float y)
{
	if (RunningProgram)
		RunningProgram->HandleClick(x, y);
}

void BasePostEffect::HandleKeyPress(unsigned char key)
{
	if (RunningProgram)
		RunningProgram->HandleKeyPress(key);
}

bool BasePostEffect::Integrate(float delta)
{
	if (RunningProgram)
		return RunningProgram->Integrate(delta);
	return false;
}

void BasePostEffect::Render(float delta)
{
	if (RunningProgram)
		RunningProgram->Render(delta);
}

void RTPassthroughPPE::Render(float delta)
{
	if (RunningProgram)
	{
		RenderTarget* rt = GRenderTargets->GetRenderTarget();
		GRenderPasses->SetRenderTarget(rt);
		RunningProgram->Render(delta);
		GRenderPasses->SetRenderTarget(nullptr);

		GRenderPasses->RenderFullScreenInverted(rt->ShaderResourceId);
	}
}