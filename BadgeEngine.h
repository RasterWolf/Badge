#pragma once
#include <cassert>


/// BadgeEngine takes the programs and runs them!
/// It handles the input and rendering main loops
class BadgeEngine
{
public:
	BadgeEngine() = default;
	~BadgeEngine() = default;

	void MainLoop();
private:
	bool bIsInitialized = false;
};