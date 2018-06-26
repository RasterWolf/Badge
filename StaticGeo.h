#pragma once

class UnitCubeGeo
{
public:
	UnitCubeGeo();
	~UnitCubeGeo();

	void Draw() const;
	unsigned int VertexBuffer;
	unsigned int IndexBuffer;
};