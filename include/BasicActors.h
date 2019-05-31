#pragma once
#include <string.h>
#include "glmMath.h"
#include "TextureManager.h"
#include "ObjModel.h"

class Actor
{
public:
	Actor()
		: Position(glm::vec2(0.0f))
		, RotationAxis(glm::vec3(0.0f,0.0f,1.0f))
		, Rotation(0.0f)
		, Scale(glm::vec3(1.0f))
	{
		RotationAxis.z = 1.0f; //so we at least have an axis
	}

	glm::vec2 Position;
	glm::vec3 RotationAxis;
	float Rotation;
	glm::vec3 Scale;
	virtual glm::mat4 ApplyTransform() const;
};

class ModelActor : public Actor
{
public:
	ModelActor(const char * model)
	{
		objModel = new ObjModel(model);
	}
	~ModelActor()
	{
		delete objModel;
	}

	ObjModel* objModel;
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
