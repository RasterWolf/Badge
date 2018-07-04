#pragma once

class UnitCubeGeo
{
public:
	UnitCubeGeo();
	~UnitCubeGeo();

	void Draw(bool bInvert = false) const;
	unsigned int VertexBuffer;
	unsigned int IndexBuffer;
	unsigned int PositionAttrib;
	unsigned int TexAttrib;
};

extern UnitCubeGeo* GUnitCube;