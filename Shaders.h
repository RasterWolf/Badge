#pragma once
#include <string.h>
#include <fstream>
#include "MyGL.h"
#include <iostream>

#include "MyGL.h"


enum ShaderProgram
{
	SP_FullScreen,
	SP_ImageBox,
	SP_MAX
};

template<typename pipeline, int MaxShaders>
class Shaders
{
public:
	void InitShaders()
	{
		for (size_t i = 0; i < pipeline::GetMaxShaders(); i++)
		{
			GLuint shader = glCreateShader(pipeline::GetShaderPipeline());

			//Read the file
			std::ifstream file;
			file.open(ShaderFiles[i], std::ifstream::in);

			bool b = file.is_open();

			if (!b)
			{
				std::cout << "Couldn't open file: " << ShaderFiles[i].c_str() << std::endl;
				continue;
			}

			file.seekg(0,file.end);
			int32_t size = file.tellg();
			file.seekg(0,file.beg);

			char* glsl = (char*)malloc(size);
			file.read(glsl, size);
			file.close();

			glShaderSource(shader,1,&glsl,&size);
			glCompileShader(shader);

			int  success;
			char infoLog[512];
			glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(shader, 512, NULL, infoLog);
				std::cout << "ERROR::SHADER::" << pipeline::GetName() << "::COMPILATION_FAILED\n" << infoLog << std::endl;
			}

			free(glsl);
			ShaderPrograms[i] = shader;

		}
	}

	void DeleteShaders()
	{
		for (size_t i = 0; i < MaxShaders; i++)
		{
			glDeleteShader(ShaderPrograms[i]);
		}
	}

	GLuint GetShaderProgram(int i) const
	{
		return ShaderPrograms[i];
	}
	~Shaders()
	{
		DeleteShaders();
	}
protected:
	std::string ShaderFiles[MaxShaders];
	GLuint ShaderPrograms[MaxShaders];
};


enum VertexShaderProgram
{
	VS_FullScreen,
	VS_ImageBox,
	VS_MAX
};

class VertexShaders : public Shaders<VertexShaders,VS_MAX>
{
//Shaders interface
public:
	VertexShaders()
	{
		ShaderFiles[VS_FullScreen] = "FullScreenVS.glsl";
		ShaderFiles[VS_ImageBox] = "ImageBoxVS.glsl";
	}
	static constexpr GLenum GetShaderPipeline()
	{
		return GL_VERTEX_SHADER;
	}

	static constexpr int GetMaxShaders() 
	{
		return VS_MAX;
	}

	static constexpr const char* GetName() 
	{
		return "VERTEX";
	}
};

enum FragmentShaderProgram
{
	FS_FullScreen,
	FS_ImageBox,
	FS_MAX
};

class FragmentShaders : public Shaders<FragmentShaders,FS_MAX>
{
//Shaders interface
public:
	FragmentShaders()
	{
		ShaderFiles[FS_FullScreen] = "FullScreenFS.glsl";
		ShaderFiles[FS_ImageBox] = "ImageBoxFS.glsl";
	}
	static constexpr GLenum GetShaderPipeline() 
	{
		return GL_FRAGMENT_SHADER;
	}

	static constexpr int GetMaxShaders() 
	{
		return FS_MAX;
	}

	static constexpr const char* GetName() 
	{
		return "FRAGMENT";
	}
};

class ShaderPrograms
{
public:
	static void InitShaderPrograms();
	static void DeleteShaderPrograms();

	static unsigned int SetProgram(ShaderProgram sp)
	{
		glUseProgram(AllShaderPrograms[sp]);
		return AllShaderPrograms[sp];
	}

	static GLuint GetProgram(ShaderProgram sp)
	{
		return AllShaderPrograms[sp];
	}

private:
	
	static GLuint AllShaderPrograms[SP_MAX];
};