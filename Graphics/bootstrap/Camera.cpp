#include "Camera.h"
#include <assert.h>
#include <ext.hpp>

/*	glm::mat4 _worldTransform;
	glm::mat4 _viewTransform;
	glm::mat4 _projectionTransform;
	glm::mat4 _viewProjectionTransform;
*/


Camera::Camera() : _worldTransform(1), _viewTransform(1), _projectionTransform(1)
{
	
	
}

Camera::~Camera() {};

void Camera::update(float deltaTime)
{
	update(deltaTime);
}

void Camera::setPerspective(float FOV, float aspectRatio, float nearClip, float farClip)
{
	//GENERATE A PROJECTION MATRIX
	float X = 1 / (aspectRatio * tanf(FOV / 2));
	float Y = 1 / (tanf(FOV / 2));
	float Z = -((farClip + nearClip) / (farClip - nearClip));
	float Zz = -(2 * (farClip * nearClip) / (farClip - nearClip));

	auto P = glm::mat4(
		glm::vec4(X, 0, 0, 0), 
		glm::vec4(0, Y, 0, 0),
		glm::vec4(0, 0, Z, -1), 
		glm::vec4(0, 0, Zz, 0));

	auto perspective = glm::perspective(FOV, aspectRatio, nearClip, farClip);
	assert(P == perspective);

	this->_projectionTransform = P;
	this->_updateProjectionViewTransform();
}

void Camera::setOrtho(float left, float right, float top, float bottom, float farClip, float nearClip)
{
	auto X = 2 / (right - left);
	auto Y = 2 / (top - bottom);
	auto Z = -2 / (farClip - nearClip);

	auto Xx = -((right + left) / (right - left));
	auto Yy = -((top + bottom) / (top - bottom));
	auto Zz = -((farClip + nearClip) / (farClip - nearClip));

	glm::mat4 Projection = glm::mat4(
		glm::vec4(X, 0, 0, 0),
		glm::vec4(0, Y, 0, 0),
		glm::vec4(0, 0, Z, 0),
		glm::vec4(Xx, Yy, Zz, 1));

	glm::mat4 glmOrtho = glm::ortho(left, right, bottom, top, nearClip, farClip);

	assert(Projection == glmOrtho);

	this->_projectionTransform = Projection;
	this->_updateProjectionViewTransform();

}

void Camera::setLookAt(glm::vec3 eye, glm::vec3 center, glm::vec3 up)
{
	//CALCULATE A FOWARD VECTOR
	//Z = NORMALIZE(EYE - CENTER)
	//X = NORMALIZE(CROSS(UP, Z))
	//Y = CROSS(Z, X)
	//MAT4 V = MAT4(VEC4(X.0, X.1, X.2, 0), VEC4(Y.0, Y.1, Y.2, 0), VEC4(Z.0, Z.1, Z.2, 0,), VEC4(0,0,0,1))
	//MAT4 T = MAT4(VEC4(1,0,0,0), VEC4(0,1,0,0), VEC4(0,0,1,0), VEC4(-EYE.X, -EYE.Y, -EYE.Z, 1))
	//MAT4 VIEW = V * T
	//MAT4 WORLD = V.INVERSE()
	auto Z = glm::normalize(eye - center);
	auto X = glm::normalize(glm::cross(up, Z));
	auto Y = glm::cross(Z, X);

	glm::mat4 V = glm::mat4(
		glm::vec4(X.x, Y.x, Z.x, 0),
		glm::vec4(X.y, Y.y, Z.y, 0),
		glm::vec4(X.z, Y.z, Z.z, 0),
		glm::vec4(0, 0, 0, 1)
	);

	glm::mat4 Translation = glm::mat4(
		glm::vec4(1, 0, 0, 0),
		glm::vec4(0, 1, 0, 0),
		glm::vec4(0, 0, 1, 0),
		glm::vec4(-eye.x, -eye.y, -eye.z, 1)
	);

	auto newCam = V * Translation;
	auto glmCam = glm::lookAt(eye, center, up);
	assert(newCam == glmCam);
	
	this->_viewTransform = newCam;
	this->_worldTransform = glm::inverse(this->_viewTransform);
	
	if(this->_projectionTransform != glm::mat4(1))
		this->_updateProjectionViewTransform();
}

void Camera::setPosition(glm::vec3 position)
{
	auto Translation = glm::mat4(glm::vec4(1, 0, 0, 0), glm::vec4(0, 1, 0, 0),
		glm::vec4(0, 0, 1, 0), glm::vec4(position.x, position.y, position.z, 1));

	//this->_viewTransform = this->_viewTransform * Translation;

	this->_viewTransform = Translation;
	this->_worldTransform = glm::inverse(this->_viewTransform);
}

void Camera::setWorldTransform(glm::mat4 world) 
{
	this->_worldTransform = world;
	this->_viewTransform = glm::inverse(this->_worldTransform);
	this->_updateProjectionViewTransform();
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
	this->_updateProjectionViewTransform();
	return this->_viewProjectionTransform;
}

void Camera::_updateProjectionViewTransform()
{
	//this->_viewProjectionTransform = this->_projectionTransform * glm::inverse(this->_worldTransform);
	this->_viewProjectionTransform = this->_projectionTransform * this->_viewTransform;
}