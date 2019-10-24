#include <iostream>
#include "StaticGeo.h"
#include "Shaders.h"
#include "MyGL.h"

UnitCubeGeo* GUnitCube = nullptr;

float vertices[] = {
    // positions          // Tex Coord
     1.0f,  1.0f, 0.0f,   1.0f, 0.0f,   // top right
     1.0f, -1.0f, 0.0f,   1.0f, 1.0f,   // bottom right
    -1.0f, -1.0f, 0.0f,   0.0f, 1.0f,   // bottom left
    -1.0f,  1.0f, 0.0f,   0.0f, 0.0f    // top left
};

float vertices2[] = {
	// positions          // Tex Coord
	1.0f,  1.0f, 0.0f,   1.0f, 0.0f,   //0
	1.0f, -1.0f, 0.0f,   1.0f, 1.0f,   //1
	-1.0f,  1.0f, 0.0f,   0.0f, 0.0f,   //3

	1.0f, -1.0f, 0.0f,   1.0f, 1.0f,   //1
	-1.0f, -1.0f, 0.0f,   0.0f, 1.0f,  //2
	-1.0f,  1.0f, 0.0f,   0.0f, 0.0f,  //3	

	// positions          // Tex Coord
	1.0f,  1.0f, 0.0f,   1.0f, 1.0f,   //0
	1.0f, -1.0f, 0.0f,   1.0f, 0.0f,   //1
	-1.0f,  1.0f, 0.0f,   0.0f, 1.0f,   //3

	1.0f, -1.0f, 0.0f,   1.0f, 0.0f,   //1
	-1.0f, -1.0f, 0.0f,   0.0f, 0.0f,  //2
	-1.0f,  1.0f, 0.0f,   0.0f, 1.0f,  //3	
};

unsigned int indices[] = {  // note that we start from 0!
    0, 1, 3,   // first triangle
    1, 2, 3    // second triangle
};

UnitCubeGeo::UnitCubeGeo()
{
	unsigned int PositionAttrib = ShaderPrograms::GetVertexAttrib(ShaderProgram::SP_FullScreen, "aPos");
	unsigned int TexAttrib = ShaderPrograms::GetVertexAttrib(ShaderProgram::SP_FullScreen, "aTexCoord");
	GL_ASSERT;

	std::cout << "Creating Unit Cube with Pos in: " << PositionAttrib << std::endl;
	std::cout << "Creating Unit Cube with tex in: " << TexAttrib << std::endl;

	const auto VBSize = sizeof(vertices2);
	const auto stride = sizeof(float) * 5;
	const auto TexOffset = sizeof(float) * 3;
	

	VertexBuffer.InitData((void*)&vertices2[0], VBSize);
	GlVbAttrib Atrribs[] = {{ PositionAttrib ,3	,stride ,0 },
							{ TexAttrib		 ,2	,stride,(void*)TexOffset} };

	VertexBuffer.AddAttrib(Atrribs[0]);
	VertexBuffer.AddAttrib(Atrribs[1]);

	IndexBuffer.InitData(indices, sizeof(indices));
	GL_ASSERT;

}

UnitCubeGeo::~UnitCubeGeo()
{
}

void UnitCubeGeo::Draw(bool bInvert) const
{
	VertexBuffer.Bind();
	GL_ASSERT;
	IndexBuffer.Bind();
	GL_ASSERT;

	if (!bInvert)
	{
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 6);
		GL_ASSERT;
	}
	else
	{
		glDrawArrays(GL_TRIANGLE_STRIP, 6, 6);
		GL_ASSERT;
	}
}
