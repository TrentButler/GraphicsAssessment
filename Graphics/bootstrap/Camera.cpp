#include "Camera.h"
#include <glm.hpp>


/*	glm::mat4 _worldTransform;
	glm::mat4 _viewTransform;
	glm::mat4 _projectionTransform;
	glm::mat4 _viewProjectionTransform;
*/


Camera::Camera() 
{
	//INITLIZE THE CAMERA'S TRANSFOMRMS
	this->_worldTransform = glm::mat4(1);
	this->_viewTransform = glm::mat4(1);
	this->_projectionTransform = glm::mat4(1);
	this->_viewProjectionTransform = glm::mat4(1);
}

Camera::~Camera() {};

void Camera::update(float deltaTime) {};

void Camera::setPerspective(float FOV, float aspectRatio, float nearClip, float farClip) {};

void Camera::setLookAt(glm::vec3 center, glm::vec3 target, glm::vec3 direction) 
{
	//NEEDS WORK
	//STUDY ROTATIONS
};

void Camera::setPosition(glm::vec3 position) 
{
	this->_viewTransform = glm::mat4(glm::vec4(1, 0, 0, 0), glm::vec4(0, 1, 0, 0),
		glm::vec4(0, 0, 1, 0), glm::vec4(position.x, position.y, position.z, 1));
}

glm::mat4 Camera::getWorldTransform() 
{
	return this->_worldTransform;
}

glm::mat4 Camera::getView() 
{
	return this->_viewTransform;
}

glm::mat4 Camera::getProjection() 
{
	return this->_projectionTransform;
}

glm::mat4 Camera::getProjectionView() 
{
	return this->_viewProjectionTransform;
}

void Camera::_updateProjectionViewTransform() 
{
	this->_viewProjectionTransform = this->_projectionTransform * this->_viewTransform;
}