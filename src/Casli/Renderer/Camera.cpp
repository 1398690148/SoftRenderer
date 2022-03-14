#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <Platform/CasliMath.h>
#include <algorithm>

Camera::Camera()
{

}

Camera::Camera(glm::vec3 e, glm::vec3 c, glm::vec3 up)
	: eye(e), center(c), up(up), yaw(0), pitch(0)
{
}

const glm::vec3 Camera::GetPosition() const
{
	return eye;
}

glm::mat4 Camera::GetMatrix()
{
	return glm::lookAt(eye, center, up);
}

void Camera::Translate(float t)
{
	eye = eye + eye * t * travelSpeed;
}

void Camera::Rotate(float dx, float dy)
{
	//yaw = dy * rotationSpeed;
	pitch = dx * rotationSpeed;
	glm::mat4 rotate = glm::mat4(1.0);
	rotate = glm::rotate(rotate, glm::radians(-pitch), glm::vec3(0, 1, 0));
	eye = rotate * glm::vec4(eye, 1.0);
}