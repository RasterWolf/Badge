#pragma once
#include <string>
#include "Vectors.h"
#include "GLBuffers.h"

using namespace std;

class ObjModel
{
public:
	ObjModel(const char* model);
	~ObjModel();

	void Draw() const;
	GlVertexBuffer VertexBuffer;
	GlIndexBuffer IndexBuffer;

private:
	unsigned int NumIndicies;
};