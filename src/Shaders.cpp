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
		bool					bEnable;
	};

	ProgramDef ProgramDefinitions[] = { {FS_FullScreen,VS_FullScreen,true},
	{ FS_ImageBox,VS_ImageBox ,true},
	{ FS_TvDistortion,VS_FullScreen,true },
	{ FS_Chromatical,VS_FullScreen,true },
	{ FS_GreenTV,VS_FullScreen,true },
	{ FS_Sketch, VS_FullScreen ,true}
	,{ FS_Sketch2, VS_FullScreen,true }
	,{ FS_Fractal, VS_FullScreen,false } //this fractal shader is killing the compiler on the pi.
	};

	for(int i = 0 ; i < SP_MAX ; ++i)
	{
		if (!ProgramDefinitions[i].bEnable)
			continue;

		std::cout << "creating program: " << i << std::endl;
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

