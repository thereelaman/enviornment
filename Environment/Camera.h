#pragma once
#include <math.h>
#include <glut.h>

class Camera
{
private:
	// location
	float xLoc = 0.0f,
		yLoc = 0.0f,
		zLoc = 10.0f;

	// rotation
	float xRot = 0.0f,
		yRot = 0.0f,
		zRot = 0.0f;

	//up vector i.e. what is the the up/roof/top of the camera
	float xUp = 0.0f,
		yUp = 0.0f,
		zUp = 0.0f;

	Camera();
	~Camera();

public:

	void updateCameraLocation(float x, float y, float z);
	//each element should range from 0 to 360 degrees.
	void updateCameraRotation(float roll, float pitch, float yaw);
	void updateCameraUpVector(float x, float y, float z);
	void setCameraUpVector(float x, float y, float z);

};

