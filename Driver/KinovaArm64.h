#include <vector>

#define EXPORT __attribute__((visibility("default")))

extern "C"
{	
	EXPORT int InitRobot();
	EXPORT int MoveArmHome(bool rightArm);
	EXPORT int MoveHand(bool rightArm, float x, float y, float z, float thetaX, float thetaY, float thetaZ);
	EXPORT int MoveHandNoThetaY(bool rightArm, float x, float y, float z, float thetaX, float thetaZ);
	EXPORT int MoveFingers(bool rightArm, bool pinky, bool ring, bool middle, bool index, bool thumb);
	EXPORT int StopArm(bool rightArm);
	EXPORT int CloseDevice(bool rightArm);
	//Shawn's f()'s
	EXPORT int MoveArmAngularVelocity(bool rightArm, float av1, float av2, float av3, float av4, float av5, float av6, float av7);
	EXPORT int MoveArmAngularPosition(bool rightArm, float ap1, float ap2, float ap3, float ap4, float ap5, float ap6, float ap7);
	EXPORT int CartesianPosition_MoveRelative(bool rightArm, float X, float Y, float Z, float ThetaX, float ThetaY, float ThetaZ);
	EXPORT int MoveArmAngularVelocityLooped(bool rightArm, int iterations, float av1, float av2, float av3, float av4, float av5, float av6, float av7);
}



