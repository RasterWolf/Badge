#include "TextureManager.h"
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
	SDL_Surface* originalSurface = IMG_Load(file);


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




void TextureManager::InitTextureManager()
{
	DIR *dir = opendir ("./");
	struct dirent *ent;
	if (dir != nullptr) 
	{
	  while ((ent = readdir (dir)) != nullptr) 
	  {
	  	int len = strlen(ent->d_name);
	  	std::string fileName(ent->d_name);
	  	std::transform(fileName.begin(), fileName.end(), fileName.begin(), ::tolower);
	  	bool IsImageFile = fileName.find(".png") != std::string::npos 
	  					|| fileName.find(".bmp") != std::string::npos
	  					|| fileName.find(".tga") != std::string::npos;

	  	if(IsImageFile)
		{
			LoadedTextures[std::string(ent->d_name)] = ReadTextureFromFile(ent->d_name);;
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

    return BadgeImage(); //intentional, go to max
}
