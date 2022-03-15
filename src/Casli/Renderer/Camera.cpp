#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <Platform/CasliMath.h>
#include <algorithm>
#include <glm/vec4.hpp>
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
	glm::vec4 forwardBaseVector = glm::vec4(0.0f, 0.0f, -1.0f, 0.0f);
	// apply the camera rotations to a base vector
	glm::mat4 rotate = glm::mat4(1.0);
	rotate = glm::rotate(rotate, -yaw, glm::vec3(0, 1.0, 0));
	rotate = glm::rotate(rotate, -pitch, glm::vec3(1.0, 0, 0));
	const glm::vec3 lookVector = rotate * forwardBaseVector;

	return glm::lookAt(eye, eye + lookVector, up);
}

//void Camera::Translate(float t)
//{
//	eye = eye + eye * t * travelSpeed;
//}

void Camera::Translate(glm::vec3 translation)
{
	glm::mat4 matrix = glm::mat4(1.0);
	matrix = glm::rotate(matrix, -yaw, glm::vec3(0, 1.0, 0));
	matrix = glm::rotate(matrix, -pitch, glm::vec3(1.0, 0, 0));
	matrix = glm::scale(matrix, glm::vec3(travelSpeed, travelSpeed, travelSpeed));
	translation = matrix * glm::vec4(translation, 1.0);
	eye = eye + translation;
}

//void Camera::Rotate(float dx, float dy)
//{
//	yaw = dy * rotationSpeed;
//	pitch = dx * rotationSpeed;
//	glm::mat4 rotate = glm::mat4(1.0);
//	rotate = glm::rotate(rotate, glm::radians(-yaw), glm::vec3(1, 0, 0));
//	rotate = glm::rotate(rotate, glm::radians(-pitch), glm::vec3(0, 1, 0));
//	eye = rotate * glm::vec4(eye, 1.0);
//}

void Camera::Rotate(float dx, float dy)
{
	yaw = yaw + dx * rotationSpeed;
	pitch = std::clamp(pitch + dy * rotationSpeed, 0.995f * -PI / 2.0f, 0.995f * PI / 2.0f);
}