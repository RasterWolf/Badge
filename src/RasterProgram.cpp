#include "RasterProgram.h"
#include "GpioControls.h"

//Main Badge Program

RasterProgram::RasterProgram() : 
	ButtonClickCount()
{
	bForceNextFrameRender = false;

	//Programs
	BadgePrograms.Programs.push_back(new RasterBadge());
	BadgePrograms.Programs.push_back(new PulexBadge());
	BadgePrograms.Programs.push_back(new BasicBadge("KBadge.png"));

	//Post Effects
	PostPrograms.Programs.push_back(new RTPassthroughPPE());
	PostPrograms.Programs.push_back(new STPostEffect(ShaderProgram::SP_TvDistortion));
	PostPrograms.Programs.push_back(new STPostEffect(ShaderProgram::SP_Chromatical));
	PostPrograms.Programs.push_back(new STPostEffect(ShaderProgram::SP_GreenTV));
	PostPrograms.Programs.push_back(new STPostEffect(ShaderProgram::SP_Sketch));
	PostPrograms.Programs.push_back(new STPostEffect(ShaderProgram::SP_Sketch2));
	//PostPrograms.Programs.push_back(new STPostEffect(ShaderProgram::SP_Fractal));


	auto* program = BadgePrograms.AdvanceProgram();
	auto* post = PostPrograms.AdvanceProgram();

	CurrentProgram = post;
	post->SetProgram(program);
}

RasterProgram::~RasterProgram()
{
	
}

const float BrightnessValues[] = { 1.0,0.5f,0.4f,0.1f };
const int nBrightness = sizeof(BrightnessValues) / sizeof(float);
int brightnessIndex = 0;

void RasterProgram::HandleClick(float x, float y)
{
	
	int i = UpdateInvisibleButtonClick(x, y);
	if (i != -1)
	{
		switch (i)
		{
		case 0:
			//SetPrograms(PostPrograms.GetCurrentProgram(),BadgePrograms.PreviousProgram());
			exit(0);
			break;
		case 1:
			//SetPrograms(PostPrograms.GetCurrentProgram(), BadgePrograms.AdvanceProgram());
			brightnessIndex = (brightnessIndex + 1) % nBrightness;
			GpioControls::SetBrightness(BrightnessValues[brightnessIndex]);
			break;
		case 2:
			SetPrograms(PostPrograms.PreviousProgram(), BadgePrograms.GetCurrentProgram());
			break;
		case 3:
			SetPrograms(PostPrograms.AdvanceProgram(), BadgePrograms.GetCurrentProgram());
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