#pragma once
#include <RendererAPI.h>
#include "Graphics.h"

class RENDERER_API Camera
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