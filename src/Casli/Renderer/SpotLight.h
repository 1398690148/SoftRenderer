#pragma once
#include "PointLight.h"

class SpotLight : public PointLight
{
public:
	SpotLight(Graphics &gfx, glm::vec3 color, glm::vec3 position, glm::vec3 direction, float constant, float linear, float exp, float cutoff, unsigned int offset);
	void Bind(Graphics &gfx);
private:
	float Cutoff;
	glm::vec3 direction;
};