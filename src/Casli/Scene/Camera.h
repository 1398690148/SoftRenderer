#pragma once
#include "Graphics.h"

class Camera
{
public:
	Camera(glm::vec3 e, glm::vec3 cen, glm::vec3 up, float aspect);
	glm::mat4x4 GetMatrix();
	void Translate(glm::vec3 offset);
private:
	glm::vec3 eye;
	glm::vec3 center;
	glm::vec3 up;
	float aspect;
};