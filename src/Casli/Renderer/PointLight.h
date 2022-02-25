#pragma once
#include "Light.h"

class PointLight : public Light
{
public:
	PointLight();
	PointLight(Graphics &gfx, glm::vec3 color, glm::vec3 position, float constant, float linear, float exp, unsigned int offset);
	void Bind(Graphics &gfx);
protected:
	glm::vec3 position;
	float Constant;
	float Linear;
	float Exp;
};