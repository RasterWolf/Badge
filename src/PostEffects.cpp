#include "PostEffects.h"
#include "RenderPasses.h"
#include "BadgeEngine.h"

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

STPostEffect::STPostEffect(ShaderProgram shaderProgram)
	: programUsed(shaderProgram)
{
	timeParameter = ShaderPrograms::GetUniformLocation(shaderProgram, "iTime");
	resolutionParameter = ShaderPrograms::GetUniformLocation(shaderProgram, "iResolution");

	channelParameters[0] = ShaderPrograms::GetUniformLocation(shaderProgram, "iChannel0");
	channelParameters[1] = ShaderPrograms::GetUniformLocation(shaderProgram, "iChannel1");
	channelParameters[2] = ShaderPrograms::GetUniformLocation(shaderProgram, "iChannel2");
	channelParameters[3] = ShaderPrograms::GetUniformLocation(shaderProgram, "iChannel3");

	channelParameterValues[0] = 0;
	channelParameterValues[1] = 0;
	channelParameterValues[2] = 0;
	channelParameterValues[3] = 0;
}

STPostEffect::~STPostEffect()
{

}

void STPostEffect::Render(float delta)
{
	if (RunningProgram)
	{
		RenderTarget* rt = GRenderTargets->GetRenderTarget();
		GRenderPasses->SetRenderTarget(rt);
		RunningProgram->Render(delta);
		GRenderPasses->SetRenderTarget(nullptr);

		//uniforms
		channelParameterValues[0] = rt->ShaderResourceId;

		ShaderPrograms::SetProgram(programUsed);
		if (timeParameter >= 0)
			glUniform1f(timeParameter, GEngine.GetTimeSeconds());
		if (resolutionParameter >= 0)
			glUniform2f(resolutionParameter, (float)GEngine.GetWidth(), (float)GEngine.GetHeight());

		for (int i = 0; i < NumChannels; i++)
		{
			if (channelParameters[i] >= 0)
			{
				glActiveTexture(GL_TEXTURE0 + channelParameters[i]);
				glBindTexture(GL_TEXTURE_2D, channelParameterValues[i]);
				GL_ASSERT;
			}
			
		}
		
		GL_ASSERT;

		GUnitCube->Draw(true);
		GL_ASSERT;

		glActiveTexture(GL_TEXTURE0);

	}
}

bool STPostEffect::Integrate(float delta)
{
	if (RunningProgram)
		RunningProgram->Integrate(delta);
	return true;
}
