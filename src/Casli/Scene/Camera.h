#pragma once
#include "Graphics.h"

class Camera
{
public:
	Camera(glm::vec3 e, glm::vec3 cen, glm::vec3 up, float aspect);
	glm::mat4x4 GetMatrix();
	//void SpawnControlWindow();
	//void Reset();
private:
	//Matrix GetTranslateMatrix();
	//Matrix GetRotationMatrix();
private:
	glm::vec3 eye;
	glm::vec3 center;
	glm::vec3 up;
	float aspect;
};