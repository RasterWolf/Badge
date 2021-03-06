﻿#include "TextureManager.h"
#include "Platform.h"
#include <map>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <algorithm>
#include "dirent.h"

#include "MyGL.h"


std::map<std::string,BadgeImage> LoadedTextures;

BadgeImage ReadTextureFromFile(const char* file)
{
	BadgeImage res;
#if __SWITCH__
	FILE* f = fopen(file, "rb");
	SDL_RWops *rw = SDL_RWFromFP(f, SDL_TRUE);
	SDL_Surface* originalSurface = IMG_Load_RW(rw,1);
#else
	SDL_Surface* originalSurface = IMG_Load(file);
#endif

	if (!originalSurface)
	{
		
		std::cout << "Couldn't open image file: " << file << std::endl;
		std::cout << "Error: " << IMG_GetError() << std::endl;
		res.Width = 0;
		res.Height = 0;
		res.TextureId = 0;
		return res;
	}

	int Mode = GL_RGB;
 
	if(originalSurface->format->BytesPerPixel == 4) {
	    Mode = GL_RGBA;
	}

	GLuint ret;

	glGenTextures( 1, &ret );
	glBindTexture( GL_TEXTURE_2D, ret );
	glTexImage2D( GL_TEXTURE_2D, 0, Mode, originalSurface->w, originalSurface->h, 0, Mode, GL_UNSIGNED_BYTE, originalSurface->pixels );

	GLenum err = glGetError();
	if(err != GL_NO_ERROR)
	{
		std::cout << "had issues loading: " << file << std::endl;
	}

	res.Width = originalSurface->w;
	res.Height = originalSurface->h;
	res.TextureId = ret;

	//Set texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);
	SDL_FreeSurface(originalSurface);
	//SDL_FreeSurface(newSurface);
	return res;
}



#include "Constant.h"

void TextureManager::InitTextureManager()
{
	char Dir[128] = { 0 };
	sprintf(Dir, "%s%s", GPlatform.GetAppPath(), "content/");
	DIR *dir = opendir (Dir);
	char FullFileName[512] = { 0 };
	struct dirent *ent;
	if (dir != nullptr) 
	{
	  while ((ent = readdir (dir)) != nullptr) 
	  {
	  	std::string fileName(ent->d_name);
	  	std::transform(fileName.begin(), fileName.end(), fileName.begin(), ::tolower);
	  	bool IsImageFile = fileName.find(".png") != std::string::npos 
	  					|| fileName.find(".bmp") != std::string::npos
	  					|| fileName.find(".tga") != std::string::npos;

	  	if(IsImageFile)
		{
			sprintf(FullFileName, "%s%s", Dir, ent->d_name);
			LoadedTextures[std::string(ent->d_name)] = ReadTextureFromFile(FullFileName);;
		}
	    //printf ("%s\n", ent->d_name);
	  }
	  closedir (dir);
	}	
}

void TextureManager::DeleteTextureManager()
{
	for (auto it = LoadedTextures.begin(); it != LoadedTextures.end(); ++it) 
	{
		glDeleteTextures(1,&it->second.TextureId);
  	}

	LoadedTextures.empty();
}

const BadgeImage& TextureManager::GetTexture(const std::string& file)
{
	auto search = LoadedTextures.find(file);
    if(search != LoadedTextures.end()) {
    	return search->second;
    };
	static BadgeImage DUMMY;
    return DUMMY;
}
