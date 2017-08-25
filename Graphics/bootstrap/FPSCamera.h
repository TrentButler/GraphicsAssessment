#pragma once
#pragma once
#include "Camera.h"

class FPSCamera : public Camera
{
public:
	FPSCamera();
	~FPSCamera();

	// Inherited via Camera
	void update(float deltaTime);
	void setSpeed(float speed);

private:
	float _speed;
	glm::vec3 _up;
};