#include "GpioControls.h"
#include <iostream>
#include <math.h>
#include <algorithm>

const int BrightnessPin = 23;
const int LowBatPin = 25;

#if _MSC_VER
void GpioControls::InitGpioControls()
{
	std::cout << "GPIO Init called"<< std::endl;
}

void GpioControls::SetBrightness(float brightness)
{
	int iVal = (int)(1024 * brightness);
	iVal = std::min(1024, std::max(iVal, 0));
	std::cout << "Set Brightness to: " << brightness << std::endl;

}

bool GpioControls::CheckLowBattery()
{
	return true;
}

void GpioControls::Shutdown()
{
	std::cout << "Shutdown called" << std::endl;

}
#else
#include <wiringPi.h>
#include <unistd.h>
#include <linux/reboot.h>
char Buffer[512] = { 0 };
void GpioControls::InitGpioControls()
{
	wiringPiSetup();
	//pinMode(BrightnessPin, PWM_OUTPUT);
	sprintf(Buffer, "gpio mode %d pwm", BrightnessPin);
	system(&Buffer[0]);

	system("gpio pwmr 1024");
	system("gpio pwmc 20");
	system("gpio pwm-ms");

	sprintf(Buffer, "gpio pwm %d 1024", BrightnessPin);
	system(&Buffer[0]);
	pinMode(LowBatPin, INPUT);
}

void GpioControls::SetBrightness(float brightness)
{
	int iVal = (int)(1024 * brightness);
	iVal = std::min(1024, std::max(iVal, 0));
	sprintf(Buffer, "gpio pwm %d %d", BrightnessPin, iVal);
	system(&Buffer[0]);
	//pwmWrite(BrightnessPin, iVal);
}

bool GpioControls::CheckLowBattery()
{
	return false;
	//return digitalRead(LowBatPin) == 0;
}

void GpioControls::Shutdown()
{
	//exit(0);
	//system("shutdown -P now");

	//rfkill block all
	//rfkill unblock all
}
#endif
