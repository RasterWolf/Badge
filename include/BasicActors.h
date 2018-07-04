#pragma once
#include <string.h>
#include "glmMath.h"
#include "TextureManager.h"

class Actor
{
public:
	Actor()
		: Position(glm::vec2(0.0f))
		, Rotation(0.0f)
		, Scale(1.0f)
	{
		;
	}

	glm::vec2 Position;
	float Rotation;
	float Scale;
	virtual glm::mat4 ApplyTransform() const;
};

class MovingActor : public Actor
{
public:
	MovingActor()
		: Inertia(glm::vec2(0.0f))
	{
		;
	}

	virtual bool Integrate(float tick);
	glm::vec2 Inertia;

};

class ImageActor : public MovingActor
{
public:
	ImageActor(const std::string& image);
	virtual void Render();
	virtual glm::mat4 ApplyTransform() const override;
	BadgeImage Image;
};
