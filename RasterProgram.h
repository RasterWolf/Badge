#pragma once
#include "BadgePrograms.h"
#include "PostEffects.h"

//Uber program that cotains the hidden buttons and cycles post effects / other badge programs
class RasterProgram : public BadgeProgram
{
public:
	RasterProgram();
	~RasterProgram();

	virtual void HandleClick(float x, float y) override;
	virtual void HandleKeyPress(unsigned char key) override;
	virtual void Render(float delta) override;
	virtual bool Integrate(float delta) override;

	//Invisible Button Mapping
	glm::ivec4 ButtonClickCount;

	//Badge Programs
	std::vector<BadgeProgram*> AllBadgePrograms;
	int CurrentProgramIndex;
	BadgeProgram* CurrentProgram;
	void AdvanceProgram()
	{
		CurrentProgramIndex += 1;
		if (CurrentProgramIndex >= (int)AllBadgePrograms.size())
			CurrentProgramIndex = 0;
		bpe.SetProgram(AllBadgePrograms[CurrentProgramIndex]);
		bForceNextFrameRender = true;
	}

	void PreviousProgram()
	{
		CurrentProgramIndex -= 1;
		if (CurrentProgramIndex < 0)
			CurrentProgramIndex = AllBadgePrograms.size() - 1;
		bpe.SetProgram(AllBadgePrograms[CurrentProgramIndex]);
		bForceNextFrameRender = true;
	}

	//Post Process Effects
	RTPassthroughPPE bpe;

protected:
	bool bForceNextFrameRender;

	int UpdateInvisibleButtonClick(float x, float y);
};