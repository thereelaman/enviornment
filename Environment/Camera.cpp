#include "Camera.h"

Camera::Camera()
{
	gluLookAt(xLoc, yLoc, zLoc,
			  xRot, yRot, zRot,
			  xUp,  yUp,  zUp);
}

Camera::~Camera()
{
}

void Camera::updateCameraLocation(float x, float y, float z)
{
	xLoc += x;
	yLoc += y;
	zLoc += z;
}

void Camera::updateCameraRotation(float roll, float pitch, float yaw)
{
	xRot += sin(yaw) - cos(roll);
	yRot += sin(roll) - cos(pitch);
	zRot += -cos(yaw) + sin(pitch);
}

void Camera::updateCameraUpVector(float x, float y, float z)
{
	xUp += x;
	yUp += y;
	zUp += z;
}

void Camera::setCameraUpVector(float x, float y, float z)
{
	xUp = x;
	yUp = y;
	zUp = z;
}
