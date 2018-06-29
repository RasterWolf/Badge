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

	ProgramDef ProgramDefinitions[] = {{FS_FullScreen,VS_FullScreen},
										{FS_ImageBox,VS_ImageBox}};

	for(int i = 0 ; i < SP_MAX ; ++i)
	{
		auto& def = ProgramDefinitions[i];
		GLuint shaderProgram = glCreateProgram();
		glAttachShader(shaderProgram, Vs.GetShaderProgram(def.VertexShader));
		glAttachShader(shaderProgram, Ps.GetShaderProgram(def.FragmentShader));
		glLinkProgram(shaderProgram);


		//int  success = 1;
		char infoLog[512] = { 0 };
		//glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
		//if(!success) 
		{
			glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
			if(infoLog[0])
			std::cout << "ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n" << infoLog << std::endl;
		    
		}

		AllShaderPrograms[i] = shaderProgram;

	}
}

void ShaderPrograms::DeleteShaderPrograms()
{
	for (size_t i = 0; i < SP_MAX; i++)
		glDeleteProgram(AllShaderPrograms[0]);
}

