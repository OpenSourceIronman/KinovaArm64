#include <iostream>
#include <dlfcn.h>
#include <vector>
#include "Kinova.API.CommLayerUbuntu.h"
#include "Kinova.API.UsbCommandLayerUbuntu.h"
#include "KinovaTypes.h"
#include "KinovaArm64.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>

KinovaDevice list[MAX_KINOVA_DEVICE];
char* leftArm = "PJ00650019161750001";
char* rightArm = "PJ00900006020921-0";
int leftArmIndex = -1;
int rightArmIndex = -1;

//Function pointers to the functions we need
int(*MyInitAPI)();
int(*MyCloseAPI)();
int(*MySendBasicTrajectory)(TrajectoryPoint command);
int(*MyGetDevices)(KinovaDevice devices[MAX_KINOVA_DEVICE], int &result);
int(*MySetActiveDevice)(KinovaDevice device);
int(*MyMoveHome)();
int(*MyInitFingers)();
int(*MyGetQuickStatus)(QuickStatus &);
int(*MyGetCartesianCommand)(CartesianPosition &);
int(*MyEraseAllTrajectories)();
int(*MyGetAngularCommand)(AngularPosition&);
void * commandLayer_handle; //Handle for the library's command layer.

using namespace std;
extern "C"
{
	int InitRobot()
	{
		//We load the library
		commandLayer_handle = dlopen("Kinova.API.USBCommandLayerUbuntu.so", RTLD_NOW | RTLD_GLOBAL);

		//We load the functions from the library (Under Windows, use GetProcAddress)
		MyInitAPI = (int(*)()) dlsym(commandLayer_handle, "InitAPI");//1
		MyCloseAPI = (int(*)()) dlsym(commandLayer_handle, "CloseAPI");//2
		MyMoveHome = (int(*)()) dlsym(commandLayer_handle, "MoveHome");//3
		MyInitFingers = (int(*)()) dlsym(commandLayer_handle, "InitFingers");//4
		MyGetDevices = (int(*)(KinovaDevice devices[MAX_KINOVA_DEVICE], int &result)) dlsym(commandLayer_handle, "GetDevices");//5
		MySetActiveDevice = (int(*)(KinovaDevice devices)) dlsym(commandLayer_handle, "SetActiveDevice");//6
		MySendBasicTrajectory = (int(*)(TrajectoryPoint)) dlsym(commandLayer_handle, "SendBasicTrajectory");//7
		MyGetQuickStatus = (int(*)(QuickStatus &)) dlsym(commandLayer_handle, "GetQuickStatus");//8
		MyGetCartesianCommand = (int(*)(CartesianPosition &)) dlsym(commandLayer_handle, "GetCartesianCommand");//9
		MyEraseAllTrajectories = (int(*)()) dlsym(commandLayer_handle, "EraseAllTrajectories");//10
		MyGetAngularCommand = (int(*)(AngularPosition &)) dlsym(commandLayer_handle, "GetAngularCommand");//11

		//Verify that all functions has been loaded correctly
			if(MyInitAPI == NULL)
		{
			return -10;
		}
		else if(MyCloseAPI == NULL)
		{
			return -11;
		}
		else if(MySendBasicTrajectory == NULL)
		{
			return -12;
		}
		else if(MyGetDevices == NULL)
		{
			return -13;
		}
		else if(MySetActiveDevice == NULL)
		{
			return -14;
		}
		else if(MyGetAngularCommand == NULL)
		{
			return -15;
		}
		else if(MyMoveHome == NULL)
		{
			return -16;
		}
		else if(MyInitFingers == NULL)
		{
			return -17;
		}

		int result = (*MyInitAPI)();

		int devicesCount = MyGetDevices(list, result);
		for (int i = 0; i < devicesCount; i++)
		{
			if (strcmp(leftArm, list[i].SerialNumber) == 0) {
				leftArmIndex = i;
			}
			else if (strcmp(rightArm, list[i].SerialNumber) == 0) {
				rightArmIndex = i;
			}
		}

		if (devicesCount >= 1)
		{
			return 0;
		}

		// not succesfull - no device found
		return - 2;
	}

	void EnableDesiredArm(bool rightArm)
	{
		if (rightArm) {
			MySetActiveDevice(list[rightArmIndex]);
		}
		else {
			MySetActiveDevice(list[leftArmIndex]);
		}
	}

	// send robot to new point
	int MoveHand(bool rightArm, float x, float y, float z, float thetaX, float thetaY, float thetaZ)
	{
		EnableDesiredArm(rightArm);
		TrajectoryPoint pointToSend;
		pointToSend.InitStruct();
		pointToSend.Position.Type = CARTESIAN_POSITION;
		pointToSend.Position.CartesianPosition.X = x;
		pointToSend.Position.CartesianPosition.Y = y;
		pointToSend.Position.CartesianPosition.Z = z;
		pointToSend.Position.CartesianPosition.ThetaX = thetaX;
		pointToSend.Position.CartesianPosition.ThetaY = thetaY;
		pointToSend.Position.CartesianPosition.ThetaZ = thetaZ;

		MySendBasicTrajectory(pointToSend);

		return 0;
	}
	//Shawn Testing :
	int MoveArmAngularVelocity(bool rightArm, float av1 = 0, float av2 = 0, float av3 = 0, float av4 = 0, float av5 = 0, float av6 = 0, float av7 = 0)
	{
		EnableDesiredArm(rightArm);
		TrajectoryPoint pointToSend;
		pointToSend.InitStruct();
		pointToSend.Position.Type = ANGULAR_VELOCITY;
		pointToSend.Position.Actuators.Actuator1 = av1;
		pointToSend.Position.Actuators.Actuator2 = av2;
		pointToSend.Position.Actuators.Actuator3 = av3;
		pointToSend.Position.Actuators.Actuator4 = av4;
		pointToSend.Position.Actuators.Actuator5 = av5;
		pointToSend.Position.Actuators.Actuator6 = av6;
		pointToSend.Position.Actuators.Actuator7 = av7;

		MySendBasicTrajectory(pointToSend);

		return 0;
	}

	int MoveArmAngularVelocityLooped(bool rightArm, int iterations, float av1 = 0, float av2 = 0, float av3 = 0, float av4 = 0, float av5 = 0, float av6 = 0, float av7 = 0) {
		
		for (int i = 0; i < iterations; i++) {
			
			MoveArmAngularVelocity(true, av1, av2, av3, av4, av5, av6, av7);

		}

		return 0;
	}
	int MoveArmAngularPosition(bool rightArm, float ap1 = 0, float ap2 = 0, float ap3 = 0, float ap4 = 0, float ap5 = 0, float ap6 = 0, float ap7 = 0)
	{
		TrajectoryPoint pointToSend;
		AngularPosition currentCommand;
		EnableDesiredArm(rightArm);
		pointToSend.InitStruct();
		pointToSend.Position.Type = ANGULAR_POSITION;
		MyGetAngularCommand(currentCommand);

		pointToSend.Position.Actuators.Actuator1 = currentCommand.Actuators.Actuator1 + ap1;
		pointToSend.Position.Actuators.Actuator2 = currentCommand.Actuators.Actuator2 + ap2;
		pointToSend.Position.Actuators.Actuator3 = currentCommand.Actuators.Actuator3 + ap3;
		pointToSend.Position.Actuators.Actuator4 = currentCommand.Actuators.Actuator4 + ap4;
		pointToSend.Position.Actuators.Actuator5 = currentCommand.Actuators.Actuator5 + ap5;
		pointToSend.Position.Actuators.Actuator6 = currentCommand.Actuators.Actuator6 + ap6;
		pointToSend.Position.Actuators.Actuator7 = currentCommand.Actuators.Actuator7 + ap7;

		MySendBasicTrajectory(pointToSend);

		return 0;
	}
	//end Shawn Testing

	int MoveArmHome(bool rightArm)
	{
		EnableDesiredArm(rightArm);
		MyMoveHome();
		return 0;
	}

	int MoveHandNoThetaY(bool rightArm, float x, float y, float z, float thetaX, float thetaZ)
	{
		EnableDesiredArm(rightArm);
		CartesianPosition currentCommand;
		//get the actual angular command of the robot.
		MyGetCartesianCommand(currentCommand);

		MoveHand(rightArm, x, y, z, thetaX, currentCommand.Coordinates.ThetaY, thetaZ);

		return 0;
	}



	/**
	* @param pinky is extended if TRUE and close otherwise
	* @param ring is extended if TRUE and close otherwise
	* @param middle is extended if TRUE and close otherwise
	* @param index is extended if TRUE and close otherwise
	* @param thumb is extended if TRUE and close otherwise
	*/
	int MoveFingers(bool rightArm, bool pinky, bool ring, bool middle, bool index, bool thumb) {
		EnableDesiredArm(rightArm);
		CartesianPosition currentCommand;
		//get the actual angular command of the robot.
		MyGetCartesianCommand(currentCommand);

		TrajectoryPoint pointToSend;
		pointToSend.InitStruct(); // initializes all values to 0.0
		pointToSend.Position.CartesianPosition.X = currentCommand.Coordinates.X;
		pointToSend.Position.CartesianPosition.Y = currentCommand.Coordinates.Y;
		pointToSend.Position.CartesianPosition.Z = currentCommand.Coordinates.Z;
		pointToSend.Position.CartesianPosition.ThetaX = currentCommand.Coordinates.ThetaX;
		pointToSend.Position.CartesianPosition.ThetaY = currentCommand.Coordinates.ThetaY;
		pointToSend.Position.CartesianPosition.ThetaZ = currentCommand.Coordinates.ThetaZ;
		float fingerValue = 0.0f;

		if (pinky && ring && middle && index && thumb) {
			//OPEN HAND CF_OpenHandOneFingers = 31, CF_OpenHandTwoFingers = 33,
			//0.0 to 10.0 are the possible finger opening steps See KinovaTypes.h line 560 (struct FingersPosition)
			fingerValue = 10.0f;
		}

		pointToSend.Position.Fingers.Finger1 = fingerValue;
		pointToSend.Position.Fingers.Finger2 = fingerValue;
		pointToSend.Position.Fingers.Finger3 = fingerValue;

		MySendBasicTrajectory(pointToSend);

		return fingerValue;

	}//END MOVEFINGER FUNCTION 

	int StopArm(bool rightArm)
	{
		EnableDesiredArm(rightArm);
		MyEraseAllTrajectories();
		return 0;
	}

	// Close device & free the library
	int CloseDevice(bool rightArm)
	{
		EnableDesiredArm(rightArm);
		(*MyCloseAPI)();
		dlclose(commandLayer_handle);

		return 0;
	}


	int CartesianPosition_MoveRelative(bool rightArm, float X, float Y, float Z, float ThetaX, float ThetaY, float ThetaZ) {
		
		
		EnableDesiredArm(rightArm);
		
		TrajectoryPoint pointToSend;
		
		CartesianPosition currentCommand;
		
		MyGetCartesianCommand(currentCommand);
		
		pointToSend.InitStruct();
		pointToSend.Position.Type = CARTESIAN_POSITION;
		pointToSend.Position.CartesianPosition.X = currentCommand.Coordinates.X + X;		
		pointToSend.Position.CartesianPosition.Y = currentCommand.Coordinates.Y + Y;
		pointToSend.Position.CartesianPosition.Z = currentCommand.Coordinates.Z + Z;
		pointToSend.Position.CartesianPosition.ThetaX = currentCommand.Coordinates.ThetaX + ThetaX;
		pointToSend.Position.CartesianPosition.ThetaY = currentCommand.Coordinates.ThetaY + ThetaY;
		pointToSend.Position.CartesianPosition.ThetaZ = currentCommand.Coordinates.ThetaZ + ThetaZ;		

		MySendBasicTrajectory(pointToSend);

		
		return 0;
	}
}
