#pragma once
#include "Light.h"

class DirectionalLight : public Light
{
public:
	DirectionalLight(Graphics &gfx, glm::vec3 color, glm::vec3 dir, unsigned int offset);
	void Bind(Graphics &gfx);
private:
	glm::vec3 direction;
};