#pragma once
#include <Light.h>

class RENDERER_API DirectionalLight : public Light
{
public:
	DirectionalLight(Graphics &gfx, glm::vec3 color, glm::vec3 dir, unsigned int slot);
	void Bind(Graphics &gfx);
private:
	glm::vec3 direction;
	unsigned int offset;
};