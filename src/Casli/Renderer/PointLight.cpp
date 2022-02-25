#include "PointLight.h"
#include "Casli/Scene/Graphics.h"
#include "PixelConstantBuffer.h"

PointLight::PointLight()
{
}

PointLight::PointLight(Graphics &gfx, glm::vec3 color, glm::vec3 position, float constant, float linear, float exp, unsigned int offset)
{
	this->color = color;
	this->position = position;
	Constant = constant;
	Linear = linear;
	Exp = exp;
	cbuf = new unsigned char[36];
	memcpy(cbuf, &color, 12);
	memcpy(cbuf + 12, &position, 12);
	memcpy(cbuf + 24, &Constant, 4);
	memcpy(cbuf + 28, &Linear, 4);
	memcpy(cbuf + 32, &Exp, 4);
	pConstantBuffer = new PixelConstantBuffer(gfx, offset, cbuf, 36);
}

void PointLight::Bind(Graphics &gfx)
{
	pConstantBuffer->Bind(gfx);
}
