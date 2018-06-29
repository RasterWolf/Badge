#pragma once

class UnitCubeGeo
{
public:
	UnitCubeGeo();
	~UnitCubeGeo();

	void Draw() const;
	unsigned int VertexBuffer;
	unsigned int IndexBuffer;
	unsigned int PositionAttrib;
	unsigned int TexAttrib;
};

extern UnitCubeGeo* GUnitCube;