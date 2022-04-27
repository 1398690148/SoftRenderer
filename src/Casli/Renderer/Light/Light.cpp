#include "Light.h"

Light::Light(Graphics & gfx, int offset, glm::vec3 color) : mesh(gfx, 1, color), offset(offset)
{
}

void Light::SetEyePos(glm::vec3 pos)
{
	eye = pos;
}
