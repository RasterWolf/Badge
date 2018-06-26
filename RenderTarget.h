#pragma once

class RenderTarget
{
public:
	RenderTarget(int SizeX, int SizeY);
	~RenderTarget();
	
	unsigned int RenderTargetId;
	unsigned int ShaderResourceId;
	int SizeX;
	int SizeY;

};