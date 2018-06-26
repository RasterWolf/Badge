#include "StaticGeo.h"
#include "MyGL.h"

#include "MyGL.h"



float vertices[] = {
    // positions          // Tex Coord
     1.0f,  1.0f, 0.0f,   1.0f, 0.0f,   // top right
     1.0f, -1.0f, 0.0f,   1.0f, 1.0f,   // bottom right
    -1.0f, -1.0f, 0.0f,   0.0f, 1.0f,   // bottom left
    -1.0f,  1.0f, 0.0f,   0.0f, 0.0f    // top left
};

unsigned int indices[] = {  // note that we start from 0!
    0, 1, 3,   // first triangle
    1, 2, 3    // second triangle
};

UnitCubeGeo::UnitCubeGeo()
{
	glGenBuffers(1,&VertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &IndexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(sizeof(float)*3));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBuffer);
}

UnitCubeGeo::~UnitCubeGeo()
{
	glDeleteBuffers(1,&VertexBuffer);
	glDeleteBuffers(1,&IndexBuffer);
}

void UnitCubeGeo::Draw() const
{


	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}
