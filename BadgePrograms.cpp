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

	if (TimeSinceStart >= 10000.0f)
	{
		bEnabled = false;
		return true;
	}
	return false;
}

//Raster Badge
glm::vec2 RasterBadge::NoseMin = glm::vec2(47, 221)*SCALE;
glm::vec2 RasterBadge::NoseSize = glm::vec2(61, 49)*SCALE;

void RasterBadge::HandleClick(int x, int y)
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
glm::vec2 PulexBadge::HeartMin = glm::vec2(164,260)*SCALE;
glm::vec2 PulexBadge::HeartSize = glm::vec2(64,60)*SCALE;

glm::vec3 HeartColors[] = { {1.0f,0.45f,0.55f},
							{0.1f,0.66f,0.3f},
							{1.0f,0.6,0.96f} };

void PulexBadge::HandleClick(int x, int y)
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
	glm::mat4 trans = glm::mat4(1.0f);

	trans = glm::translate(trans, glm::vec3(Position.x, Position.y, 0.0f));
	trans = glm::rotate(trans, Rotation, glm::vec3(0, 0, 1));
	trans = glm::scale(trans, glm::vec3(Image.Width*0.5f, Image.Height*0.5f, 1.0f)); //0.5 because the unit cube is -1 to 1
	trans = glm::scale(trans, glm::vec3(Scale, Scale, 1.0f));
	trans = glm::scale(trans, glm::vec3(SCALE, SCALE, 1.0f));
	GRenderPasses->RenderImageBox(Image, trans, true, color);
}