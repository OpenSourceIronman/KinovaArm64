#include <iostream>

using namespace std;
#include "../KinovaArm64/KinovaArm64.h"


int main()
{
	InitRobot();

	for (int i = 0; i < 300; i++) {
		MoveArmAngularVelocity(true, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 100.0f);
		
	}
    
	for (int i = 0; i < 10; i++) {
		CartesianPosition_MoveRelative(true, 0.0f, 0.0f, 0.0f, 0.0f, 1.57f, .0f);
	}
	CloseDevice(true);
	return 0;

}
