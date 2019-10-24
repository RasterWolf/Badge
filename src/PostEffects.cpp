#include "PostEffects.h"
#include "RenderPasses.h"
#include "BadgeEngine.h"
#include "Platform.h"

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

int BasePostEffect::Render(float delta)
{
	if (RunningProgram)
		return RunningProgram->Render(delta);
	return 0;
}

int RTPassthroughPPE::Render(float delta)
{
	if (RunningProgram)
	{
		RenderTarget* rt = GRenderTargets->GetRenderTarget();
		GRenderPasses->SetRenderTarget(rt);
		RunningProgram->Render(delta);
		return rt->ShaderResourceId;
	}
	return 0;
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

int STPostEffect::Render(float delta)
{
	if (RunningProgram)
	{
		RenderTarget* rt = GRenderTargets->GetRenderTarget();
		RenderTarget* rt2 = GRenderTargets->GetRenderTarget();
		GRenderPasses->SetRenderTarget(rt);
		{
			RunningProgram->Render(delta);
		}
		GRenderPasses->SetRenderTarget(rt2);

		//uniforms
		channelParameterValues[0] = rt->ShaderResourceId;

		int programId = ShaderPrograms::SetProgram(programUsed);

		{
			//SCOPE_TRANSFORM(glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, -1.0f, 1.0f)));

			GLuint ProjectionMatrixId = glGetUniformLocation(programId, "ObjectMat");
			glUniformMatrix4fv(ProjectionMatrixId, 1, GL_FALSE, glm::value_ptr(GRenderPasses->MatrixStackTop()));
		}
		

		if (timeParameter >= 0)
			glUniform1f(timeParameter, GPlatform.GetTimeSeconds());
		if (resolutionParameter >= 0)
			glUniform2f(resolutionParameter, (float)GEngine.GetAppWidth(), (float)GEngine.GetAppHeight());

		for (int i = 0; i < NumChannels; i++)
		{
			if (channelParameters[i] >= 0)
			{
				glUniform1i(channelParameters[i], i);
				glActiveTexture(GL_TEXTURE0 + i);
				glBindTexture(GL_TEXTURE_2D, channelParameterValues[i]);
				GL_ASSERT;
			}
			
		}
		
		GL_ASSERT;

		GUnitCube->Draw(true);
		GL_ASSERT;

		glActiveTexture(GL_TEXTURE0);

		return rt2->ShaderResourceId;
	}
	return 0;
}

bool STPostEffect::Integrate(float delta)
{
	if (RunningProgram)
		RunningProgram->Integrate(delta);
	return true;
}
