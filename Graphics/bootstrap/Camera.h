#pragma once
#include "gl_core_4_4.h"
#include <mat4x4.hpp>
#include <vec3.hpp>
#include <vec4.hpp>

//struct mat4;
//struct vec3;
class Camera
{

public:

	Camera();
	~Camera();

	virtual void update(float deltaTime) = 0;

	void setPerspective(float FOV, float aspectRatio, float nearClip, float farClip);

	void setLookAt(glm::vec3 eye, glm::vec3 center, glm::vec3 up);

	void setPosition(glm::vec3 position);

	glm::mat4 getWorldTransform();

	glm::mat4 getView();

	glm::mat4 getProjection();

	glm::mat4 getProjectionView();

private:

	void _updateProjectionViewTransform();

	glm::mat4 _worldTransform;
	glm::mat4 _viewTransform;
	glm::mat4 _projectionTransform;
	glm::mat4 _viewProjectionTransform;

};