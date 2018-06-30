#pragma once
#include "BadgePrograms.h"
#include "RenderTarget.h"

class BasePostEffect : public BadgeProgram
{
public:
	BasePostEffect() { ; }
	~BasePostEffect() { ; }

	void SetProgram(BadgeProgram* prog);

	void HandleClick(float x, float y) override;
	void HandleKeyPress(unsigned char key) override;
	bool Integrate(float delta);
	void Render(float delta);

protected:
	BadgeProgram * RunningProgram;
};

class RTPassthroughPPE : public BasePostEffect
{
public:
	void Render(float delta) override;
};
