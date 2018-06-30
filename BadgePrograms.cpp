#include "BadgePrograms.h"
#include "RenderPasses.h"
#include "TextureManager.h"
#include "Constant.h"
#include <cstdlib>

auto lerp = [](float v0, float v1, float t) {
	return v0 + t * (v1 - v0);
};

BasicBadge::BasicBadge(const std::string backgroundImage)
{
	BackgroundTexture = TextureManager::GetTexture(backgroundImage.c_str());
}

void BasicBadge::Render(float delta)
{
	GRenderPasses->RenderFullScreen(BackgroundTexture);
}

void TempImageActor::Init(const glm::vec2& pos)
{
	Position = pos;
	bEnabled = true;
	TimeSinceStart = 0.0f;
}

bool TempImageActor::Integrate(float tick)
{
	TimeSinceStart += tick;

	if (TimeSinceStart >= 5000.f)
	{
		bEnabled = false;
		return true;
	}
	return false;
}

//Raster Badge
glm::vec2 RasterBadge::NoseMin = glm::vec2(0.14f, 0.46f);
glm::vec2 RasterBadge::NoseSize = glm::vec2(0.19f, 0.1);

void RasterBadge::HandleClick(float x, float y)
{
	if (x >= NoseMin.x && x <= NoseMin.x + NoseSize.x &&
		y >= NoseMin.y && y <= NoseMin.y + NoseSize.y)
	{
		auto* bp = tiComponent.SpawnActor();
		if (bp != nullptr)
		{
			float rx = (float)rand() / RAND_MAX;
			float ry = (float)rand() / RAND_MAX;

			//116x68
			const float HalfX = 58.0f;
			const float HalfY = 34.0f;
			rx = lerp(HalfX, SIZE_X - HalfX, rx);
			ry = lerp(HalfY, SIZE_Y - HalfY, ry);

			bp->Init(glm::vec2(rx, ry));
			bp->Scale = 0.5;
		}
	}

	//special if there isn't a boop actor ?
}

void RasterBadge::Render(float delta)
{
	GRenderPasses->RenderFullScreen(BadgeFrames[tiComponent.HasActors()?1:0]);
	tiComponent.Render(delta);
}

bool RasterBadge::Integrate(float delta) 
{
	return tiComponent.Integrate(delta);
}

//Pulex Badge
glm::vec2 PulexBadge::HeartMin = glm::vec2(0.50f,0.54f);
glm::vec2 PulexBadge::HeartSize = glm::vec2(0.2f,0.125f);

glm::vec3 HeartColors[] = { {1.0f,0.45f,0.55f},
							{0.1f,0.66f,0.3f},
							{1.0f,0.6,0.96f} };

void PulexBadge::HandleClick(float x, float y)
{
	if(x >= HeartMin.x && x <= HeartMin.x + HeartSize.x &&
		y >= HeartMin.y && y <= HeartMin.y + HeartSize.y)
	{
		//Look for a free actor
		auto* bp = tiComponent.SpawnActor();
		if (bp != nullptr)
		{
			float rx = (float)rand() / RAND_MAX;
			float ry = (float)rand() / RAND_MAX;

			//116x68
			const float HalfX = 148.0f * 0.5f;
			const float HalfY = 146.0f * 0.5f;
			rx = lerp(HalfX, SIZE_X - HalfX, rx);
			ry = lerp(HalfY, SIZE_Y - HalfY, ry);

			bp->Init(glm::vec2(rx, ry));
			bp->Scale = 0.5;

			//Change the color
			const int MaxColors = sizeof(HeartColors) / sizeof(glm::vec3);
			int i = rand() % MaxColors;
			bp->color = HeartColors[i];
		}
	}

	//special if there isn't a boop actor ?
}

void PulexBadge::Render(float delta)
{
	GRenderPasses->RenderFullScreen(Background);
	tiComponent.Render(delta);
}

bool PulexBadge::Integrate(float delta)
{
	return tiComponent.Integrate(delta);
}

void HeartActor::Render()
{
	GRenderPasses->RenderImageBox(Image, ApplyTransform(), true, color);
}

//Main Badge Program

RasterProgram::RasterProgram()
{
	bForceNextFrameRender = false;

	//Programs
	AllBadgePrograms.push_back(new RasterBadge());
	AllBadgePrograms.push_back(new PulexBadge());
	AllBadgePrograms.push_back(new BasicBadge("KBadge.png"));
	CurrentProgramIndex = 0;
	CurrentProgram = AllBadgePrograms[0];
}

RasterProgram::~RasterProgram()
{
	for(auto* item : AllBadgePrograms)
		delete item;
	AllBadgePrograms.empty();
}

void RasterProgram::HandleClick(float x, float y)
{
	int i = UpdateInvisibleButtonClick(x,y);
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
			iSelect[i] = ButtonClickCount[i]+1;
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
