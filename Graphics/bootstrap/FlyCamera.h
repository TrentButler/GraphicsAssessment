#pragma once
#include "Camera.h"

class FlyCamera : public Camera
{
public:
	FlyCamera();
	~FlyCamera();

	// Inherited via Camera
	void update(float deltaTime);

private:
	float _speed;
	glm::vec3 _up;
};