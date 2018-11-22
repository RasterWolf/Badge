#pragma once

class GpioControls
{
public:
	static void InitGpioControls();
	static void SetBrightness(float brightness);
	static bool CheckLowBattery();
	static void Shutdown();
};