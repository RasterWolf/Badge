#pragma once
#include "BadgePrograms.h"
#include "RenderTarget.h"
#include "Shaders.h"

class BasePostEffect : public BadgeProgram
{
public:
	BasePostEffect() { ; }
	~BasePostEffect() { ; }

	void SetProgram(BadgeProgram* prog);

	void HandleClick(float x, float y) override;
	void HandleKeyPress(unsigned char key) override;
	bool Integrate(float delta) override;
	void Render(float delta) override;

protected:
	BadgeProgram * RunningProgram;
};

class RTPassthroughPPE : public BasePostEffect
{
public:
	void Render(float delta) override;
};

class STPostEffect : public BasePostEffect
{
public:
	STPostEffect(ShaderProgram shaderProgram);
	~STPostEffect();

	void Render(float delta) override;
	bool Integrate(float delta) override;

protected:
	static const int32_t NumChannels = 4;
	ShaderProgram programUsed;
	int32_t timeParameter;
	int32_t resolutionParameter;
	int32_t channelParameters[NumChannels];
	int32_t channelParameterValues[NumChannels];
};
