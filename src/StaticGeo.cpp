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
	PositionAttrib = ShaderPrograms::GetVertexAttrib(ShaderProgram::SP_FullScreen, "aPos");
	TexAttrib = ShaderPrograms::GetVertexAttrib(ShaderProgram::SP_FullScreen, "aTexCoord");

	std::cout << "Creating Unit Cube with Pos in: " << PositionAttrib << std::endl;
	std::cout << "Creating Unit Cube with tex in: " << TexAttrib << std::endl;

	glGenBuffers(1,&VertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);

	glGenBuffers(1, &IndexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
	glVertexAttribPointer(PositionAttrib, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(PositionAttrib);
	glVertexAttribPointer(TexAttrib, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(sizeof(float)*3));
	glEnableVertexAttribArray(TexAttrib);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBuffer);
}

UnitCubeGeo::~UnitCubeGeo()
{
	glDeleteBuffers(1,&VertexBuffer);
	glDeleteBuffers(1,&IndexBuffer);
}

void UnitCubeGeo::Draw(bool bInvert) const
{
	//glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBuffer);

	//glDrawElements(GL_TRIANGLE_STRIP, 6, GL_UNSIGNED_INT, 0);
	if (!bInvert)
	{
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 6);
	}
	else
	{
		glDrawArrays(GL_TRIANGLE_STRIP, 6, 6);

	}
	GL_ASSERT; 
}
