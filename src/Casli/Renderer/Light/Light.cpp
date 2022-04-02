#include "Light.h"

Light::Light(Graphics & gfx, int offset) : mesh(gfx, 5), offset(offset)
{
}

void Light::SetEyePos(glm::vec3 pos)
{
	eye = pos;
}
