#pragma once
#include <string>
#include "MyGL.h"


class BadgeImage
{
public:
	BadgeImage()
	: Width(0)
	, Height(0)
	, TextureId(0)
	{
		;
	}
	BadgeImage(int inWidth, int inHeight, unsigned int textureId)
		: Width(inWidth)
		, Height(inHeight)
		, TextureId(textureId)
	{
		;
	}
	int Width;
	int Height;
	unsigned int TextureId;
};

class TextureManager
{
public:
	static void InitTextureManager();
	static void DeleteTextureManager();
	static const BadgeImage& GetTexture(const std::string& file);
};