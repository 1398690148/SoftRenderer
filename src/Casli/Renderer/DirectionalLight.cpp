#include "DirectionalLight.h"
#include "Casli/Scene/Graphics.h"
#include "PixelConstantBuffer.h"

DirectionalLight::DirectionalLight(Graphics &gfx, glm::vec3 color, glm::vec3 dir, unsigned int offset)
{
	this->color = color;
	this->direction = dir;
	cbuf = new unsigned char[24];
	memcpy(cbuf, &color, 12);
	memcpy(cbuf + 12, &direction, 12);
	pConstantBuffer = new PixelConstantBuffer(gfx, offset, cbuf, 24);
}

void DirectionalLight::Bind(Graphics &gfx)
{
	pConstantBuffer->Bind(gfx);
}
