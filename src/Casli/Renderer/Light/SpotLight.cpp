#include "SpotLight.h"
#include <Graphics.h>
#include <PixelConstantBuffer.h>

SpotLight::SpotLight(Graphics &gfx, glm::vec3 color, glm::vec3 position, glm::vec3 direction, 
	float constant, float linear, float exp, float cutoff, unsigned int offset)
{
	this->color = color;
	this->position = position;
	this->direction = direction;
	this->Constant = constant;
	this->Linear = linear;
	this->Exp = exp;
	this->Cutoff = cutoff;
	cbuf = new unsigned char[52];
	memcpy(cbuf, &color, 12);
	memcpy(cbuf + 12, &position, 12);
	memcpy(cbuf + 24, &direction, 12);
	memcpy(cbuf + 36, &Constant, 4);
	memcpy(cbuf + 40, &Linear, 4);
	memcpy(cbuf + 44, &Exp, 4);
	memcpy(cbuf + 48, &Cutoff, 4);
	pConstantBuffer = new PixelConstantBuffer(gfx, offset, cbuf, 52);
}

void SpotLight::Bind(Graphics &gfx)
{
	pConstantBuffer->Bind(gfx);
}
