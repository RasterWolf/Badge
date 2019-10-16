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

	void SetPrograms(BasePostEffect* post, BadgeProgram* program)
	{
		CurrentProgram = program;
		if (post)
		{
			post->SetProgram(program);
			CurrentProgram = post;
		}
		bForceNextFrameRender = true;
	}

	//Invisible Button Mapping
	glm::ivec4 ButtonClickCount;
	BadgeProgram* CurrentProgram = nullptr;

protected:
	template<typename type>
	class CycleProgram
	{
	public:
		~CycleProgram()
		{
			/*for (auto* item : Programs)
				delete item;
			Programs.empty();*/
		}
		std::vector<type*> Programs;
		int CurrentProgramIndex = -1;
		type* AdvanceProgram()
		{
			CurrentProgramIndex += 1;
			if (CurrentProgramIndex >= (int)Programs.size())
				CurrentProgramIndex = 0;
			return Programs[CurrentProgramIndex];
		}

		type* PreviousProgram()
		{
			CurrentProgramIndex += -1;
			if (CurrentProgramIndex < 0)
				CurrentProgramIndex = Programs.size() - 1;
			return Programs[CurrentProgramIndex];
		}

		type* GetCurrentProgram() const
		{
			return Programs[CurrentProgramIndex];
		}
	};

	CycleProgram<BadgeProgram> BadgePrograms;
	CycleProgram<BasePostEffect> PostPrograms;

	bool bForceNextFrameRender;

	int UpdateInvisibleButtonClick(float x, float y);
};