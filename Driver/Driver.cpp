#include <iostream>



#include "KinovaArm64.h"
using namespace std;


int main()
{
	InitRobot();

	for (int i = 0; i < 300; i++) {
		MoveArmAngularVelocity(true, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 20.0f);
		
	}
    
	for (int i = 0; i < 10; i++) {
		CartesianPosition_MoveRelative(true, 0.0f, 0.0f, 0.0f, 0.0f, 0.57f, .0f);
	}
	CloseDevice(true);
	
	return 0;

}