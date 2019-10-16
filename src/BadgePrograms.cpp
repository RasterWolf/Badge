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

RasterBadge::RasterBadge()
	: model("Content\\Mesh.obj")
	, ImageIndex(0)
{
	BadgeFrames[0] = TextureManager::GetTexture("RasterBadge.png");
	BadgeFrames[1] = TextureManager::GetTexture("RasterBadge2.png");

	//model setup;
	const float scale = 100.5f;
	model.Rotation = 90.0f;
	model.Scale = glm::vec3(-109.0f, 96.5f, 1.0f);
	model.Position.x = SIZE_X * 0.5f;
	model.Position.y = SIZE_Y * 0.5f;
	model.RotationAxis = glm::vec3(0, 1.0f, 0);
}

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
			bp->Scale.x = 0.5f;
			bp->Scale.y = 0.5f;
		}
	}
}

void RasterBadge::Render(float delta)
{
	GRenderPasses->RenderFullScreen(BadgeFrames[tiComponent.HasActors()?1:0]);
	
	//GRenderPasses->RenderObj(*model.objModel, BadgeFrames[0], model.ApplyTransform());

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
			bp->Scale.x = 0.5f;
			bp->Scale.y = 0.5f;

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
