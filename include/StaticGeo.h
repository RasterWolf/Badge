#pragma once
#include "GLBuffers.h"

class UnitCubeGeo
{
public:
	UnitCubeGeo();
	~UnitCubeGeo();
	void Draw(bool bInvert = false) const;
	GlVertexBuffer VertexBuffer;
	GlIndexBuffer IndexBuffer;
};

extern UnitCubeGeo* GUnitCube;