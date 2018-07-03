#pragma once
#include <string.h>
#include <fstream>
#include <iostream>

#include "MyGL.h"
#include "Constant.h"


enum ShaderProgram
{
	SP_FullScreen,
	SP_ImageBox,
	SP_TvDistortion,
	SP_Chromatical,
	SP_GreenTV,
	SP_MAX
};

template<typename pipeline, int MaxShaders>
class Shaders
{
public:
	void InitShaders()
	{
		for (size_t i = 0; i < (size_t)pipeline::GetMaxShaders(); i++)
		{
			GLuint shader = glCreateShader(pipeline::GetShaderPipeline());

			//Read the file
			std::ifstream file;
			char FilePath[512] = { 0 };
			sprintf(FilePath, "%s%s", AppPath, ShaderFiles[i].c_str());

			file.open(FilePath, std::ifstream::in);

			bool b = file.is_open();

			if (!b)
			{
				std::cout << "Couldn't open file: " << ShaderFiles[i].c_str() << std::endl;
				continue;
			}

			file.seekg(0,file.end);
			int32_t size = file.tellg();
			file.seekg(0,file.beg);

			GLchar* glsl = (GLchar*)malloc(size);
			memset((void*)glsl, 0, size);
			file.read(glsl, size);
			file.close();

			glShaderSource(shader,1,(const GLchar**)&glsl,&size);
			glCompileShader(shader);
			GL_ASSERT;

			int  success = 0;
			char infoLog[512] = { 0 };
			glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(shader, 512, NULL, infoLog);
				std::cout << "ERROR::SHADER::" << pipeline::GetName() << "::COMPILATION_FAILED::" << ShaderFiles[i].c_str() << " " << success << " " << shader <<  "\n" << infoLog << std::endl;
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
	FS_TvDistortion,
	FS_Chromatical,
	FS_GreenTV,
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
		ShaderFiles[FS_TvDistortion] = "TvDistortion.glsl";
		ShaderFiles[FS_Chromatical] = "ChromaticalFS.glsl";
		ShaderFiles[FS_GreenTV] = "BadTvFS.glsl";
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

	static GLint GetVertexAttrib(ShaderProgram sp, const char* attribute)
	{
		return glGetAttribLocation(AllShaderPrograms[sp], attribute);
	}

	static GLint GetUniformLocation(ShaderProgram sp, const char* uniform)
	{
		return glGetUniformLocation(AllShaderPrograms[sp], uniform);
	}

private:
	
	static GLuint AllShaderPrograms[SP_MAX];
};