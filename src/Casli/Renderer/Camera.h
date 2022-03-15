#pragma once
#include <RendererAPI.h>
#include "Graphics.h"

class RENDERER_API Camera
{
public:
	Camera();
	Camera(glm::vec3 e, glm::vec3 c, glm::vec3 up);
	const glm::vec3 GetPosition() const;
	glm::mat4x4 GetMatrix();
	//void Translate(float t);
	void Camera::Translate(glm::vec3 translation);
	void Camera::Rotate(float dx, float dy);
private:
	glm::vec3 eye;
	glm::vec3 center;
	glm::vec3 up;
	float pitch;
	float yaw;
	glm::mat4 lookMatrix;
	static constexpr float travelSpeed = 1.f;
	static constexpr float rotationSpeed = 0.004f;
};