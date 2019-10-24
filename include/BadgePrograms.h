#pragma once
#include <string>
#include "TextureManager.h"
#include "BasicActors.h"
#include "glmMath.h"
#include "ObjModel.h"
#include <list>
#include <vector>

class BadgeProgram
{
public:
	virtual ~BadgeProgram() { ; }
	virtual void HandleClick(float x, float y) { ; }
	virtual void HandleKeyPress(unsigned char key) {;}
	virtual int Render(float delta)=0;
	virtual bool Integrate(float delta){ return false;}
};

class BasicBadge : public BadgeProgram
{
public:
	BasicBadge(const std::string backgroundImage);
	~BasicBadge() { ; }
	bool Integrate(float delta) override 
	{ 
		return BadgeProgram::Integrate(delta);
	}
	int Render(float delta) override;
private:
	BadgeImage BackgroundTexture;
};

class TempImageActor : public ImageActor
{
public:
	TempImageActor(const std::string& image) : ImageActor(image)
	{

	}
	void Init(const glm::vec2& Position);
	bool Integrate(float tick) override;
	float TimeSinceStart;
	bool bEnabled;
};

template<typename ImgType>
class TempImageComponent 
{
public:
	TempImageComponent() :
		 renderNextFrame(false)
	{
		;
	}
	void Render(float delta)
	{
		for (auto& bp : SpawnedActors)
		{
			if (bp->bEnabled)
				bp->Render();
		}
	}

	ImgType* SpawnActor()
	{
		//Look for a free actor
		for (auto& bp : AllTempActor)
		{
			if (bp.bEnabled == false)
			{
				SpawnedActors.push_back(&bp);
				renderNextFrame = true;
				return &bp;
			}
		}

		return nullptr;
	}

	bool Integrate(float delta)
	{
		bool b = renderNextFrame;
		renderNextFrame = false;
		//b |= super::Integrate(delta);
		auto i = SpawnedActors.begin();
		while (i != SpawnedActors.end())
		{
			if ((*i)->bEnabled)
			{
				b |= (*i)->Integrate(delta);
				if (!(*i)->bEnabled)
					SpawnedActors.erase(i++);  // alternatively, i = items.erase(i);
				else
					i++;
			}

			renderNextFrame = SpawnedActors.size() == 0;
		}

		return b;
	}

	bool HasActors() const
	{
		return SpawnedActors.size() > 0;
	}
protected:
	ImgType AllTempActor[10];
	std::list<TempImageActor*> SpawnedActors;
	bool renderNextFrame;


};

//Raster

class BoopActor : public TempImageActor
{
public:
	BoopActor()
	: TempImageActor("BoopBubble.png")
	{
		bEnabled = false;
		TimeSinceStart = 0.0f;
	}
};

class RasterBadge : public BadgeProgram
{
public:
	RasterBadge();
	~RasterBadge() { ; }
	void HandleClick(float x, float y) override;
	int Render(float delta) override;
	bool Integrate(float delta) override;
private:
	static glm::vec2 NoseMin;
	static glm::vec2 NoseSize;
	TempImageComponent<BoopActor> tiComponent;
	BadgeImage BadgeFrames[2];
	ModelActor model;
	int ImageIndex;
};

//Pulex

class HeartActor : public TempImageActor
{
public:
	HeartActor()
		: TempImageActor("Heart.png")
		, color(1.0f)
	{
		bEnabled = false;
		TimeSinceStart = 0.0f;
	}

	void Render() override;
	glm::vec3 color;
};

class PulexBadge : public BadgeProgram
{
public:
	PulexBadge() 
	{
		Background = TextureManager::GetTexture("PulexBadge.png");
	}
	~PulexBadge() { ; }
	void HandleClick(float x, float y) override;
	int Render(float delta) override;
	bool Integrate(float delta) override;
private:
	static glm::vec2 HeartMin;
	static glm::vec2 HeartSize;
	BadgeImage Background;
	TempImageComponent<HeartActor> tiComponent;
};
