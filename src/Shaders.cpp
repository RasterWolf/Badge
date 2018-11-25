#include "Shaders.h"

#include "MyGL.h"

GLuint ShaderPrograms::AllShaderPrograms[SP_MAX];

void ShaderPrograms::InitShaderPrograms()
{
	VertexShaders Vs;
	FragmentShaders Ps;
	Vs.InitShaders();
	Ps.InitShaders();

	struct ProgramDef
	{
		FragmentShaderProgram 	FragmentShader;
		VertexShaderProgram		VertexShader;
	};

	ProgramDef ProgramDefinitions[] = { {FS_FullScreen,VS_FullScreen},
	{ FS_ImageBox,VS_ImageBox },
	{ FS_TvDistortion,VS_FullScreen },
	{ FS_Chromatical,VS_FullScreen },
	{ FS_GreenTV,VS_FullScreen },
	{ FS_Sketch, VS_FullScreen }
	,{ FS_Sketch2, VS_FullScreen }
	,{ FS_Fractal, VS_FullScreen }
	};

	for(int i = 0 ; i < SP_MAX ; ++i)
	{
		auto& def = ProgramDefinitions[i];
		GLuint shaderProgram = glCreateProgram();
		glAttachShader(shaderProgram, Vs.GetShaderProgram(def.VertexShader));
		glAttachShader(shaderProgram, Ps.GetShaderProgram(def.FragmentShader));
		glLinkProgram(shaderProgram);
		GL_ASSERT;

		int  success = 0;
		char infoLog[512] = { 0 };
		glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
		if(success == GL_FALSE) 
		{
			glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
			//if(infoLog[0])
			std::cout << "ERROR::SHADER::PROGRAM::COMPILATION_FAILED " << success << " " << shaderProgram <<  "\n" << infoLog << std::endl;
		    
		}

		AllShaderPrograms[i] = shaderProgram;

	}
}

void ShaderPrograms::DeleteShaderPrograms()
{
	for (size_t i = 0; i < SP_MAX; i++)
		glDeleteProgram(AllShaderPrograms[0]);
}

