#include "Camera.h"
#include <glm.hpp>

Camera::Camera() {};
Camera::~Camera() {};

void Camera::update(float deltaTime) {};

void Camera::setPerspective(float FOV, float aspectRatio, float nearClip, float farClip) {};

void Camera::setLookAt(glm::vec3 center, glm::vec3 target, glm::vec3 direction) {};

void Camera::setPosition(glm::vec3 position) {};

glm::mat4 Camera::getWorldTransform() {};

glm::mat4 Camera::getView() {};

glm::mat4 Camera::getProjection() {};

glm::mat4 Camera::getProjectionView() {};

void Camera::updateProjectionViewTransform() {};