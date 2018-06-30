#include "RasterProgram.h"

//Main Badge Program

RasterProgram::RasterProgram()
{
	bForceNextFrameRender = false;

	//Programs
	AllBadgePrograms.push_back(new RasterBadge());
	AllBadgePrograms.push_back(new PulexBadge());
	AllBadgePrograms.push_back(new BasicBadge("KBadge.png"));
	CurrentProgramIndex = 0;
	CurrentProgram = &bpe;
	bpe.SetProgram(AllBadgePrograms[0]);
}

RasterProgram::~RasterProgram()
{
	for (auto* item : AllBadgePrograms)
		delete item;
	AllBadgePrograms.empty();
}

void RasterProgram::HandleClick(float x, float y)
{
	int i = UpdateInvisibleButtonClick(x, y);
	if (i != -1)
	{
		switch (i)
		{
		case 0:
			PreviousProgram();
			break;
		case 1:
			AdvanceProgram();
			break;
		case 2:
			PreviousProgram();
			break;
		case 3:
			AdvanceProgram();
			break;
		default:
			break;
		}
	}
	else
	{
		CurrentProgram->HandleClick(x, y);
	}
}

void RasterProgram::HandleKeyPress(unsigned char key)
{
	CurrentProgram->HandleKeyPress(key);
}

void RasterProgram::Render(float delta)
{
	CurrentProgram->Render(delta);
}

bool RasterProgram::Integrate(float delta)
{
	bool tmp = bForceNextFrameRender;
	bForceNextFrameRender = false;
	return CurrentProgram->Integrate(delta) || tmp;
}

int RasterProgram::UpdateInvisibleButtonClick(float x, float y)
{
	const float ButtonSize = 0.1f;
	const int NumClicks = 4;
	const float XLimit = 1.f - ButtonSize;
	const float YLimit = 1.f - ButtonSize;

	glm::ivec4 iSelect(0);
	int ButtonIndex = -1;

	const glm::vec4 ButtonLocations[] = { glm::vec4(0,0,ButtonSize,ButtonSize),
		glm::vec4(XLimit,0,1.f,ButtonSize),
		glm::vec4(0,YLimit,ButtonSize,1.f),
		glm::vec4(XLimit,YLimit,1.f,1.f) };

	for (int i = 0; i < sizeof(ButtonLocations) / sizeof(ButtonLocations[0]); i++)
	{
		const auto& item = ButtonLocations[i];
		if ((x >= item.x && x <= item.z) &&
			(y >= item.y && y <= item.w))
		{
			iSelect[i] = ButtonClickCount[i] + 1;
			ButtonIndex = i;
			break;
		}
	}

	ButtonClickCount = iSelect;

	if (ButtonIndex != -1 && ButtonClickCount[ButtonIndex] >= NumClicks)
	{
		ButtonClickCount[ButtonIndex] = 0;
		return  ButtonIndex;
	}

	return -1;
}