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

class RenderTargetFlipFlop
{
public:
	RenderTargetFlipFlop();
	~RenderTargetFlipFlop();
	RenderTarget * GetRenderTarget();
protected:
	int RtIndex = 0;
	RenderTarget* SceneTargets[2];
};

extern RenderTargetFlipFlop* GRenderTargets;