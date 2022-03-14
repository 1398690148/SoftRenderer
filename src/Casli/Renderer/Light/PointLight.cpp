#include "PointLight.h"
#include <Graphics.h>
#include <PixelConstantBuffer.h>
#include <glm/gtx/transform.hpp>

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
	pConstantBuffer = new PixelConstantBuffer(gfx, cbuf, offset, 36);
}

void PointLight::Bind(Graphics &gfx)
{
	pConstantBuffer->Bind(gfx);
}

void PointLight::rotate(float angle, glm::vec3 axis)
{
	glm::mat4 matrix = glm::mat4(1.0);
	matrix = glm::rotate(matrix, glm::radians(angle), axis);
	position = matrix * glm::vec4(position, 1.0);
	memcpy(cbuf + 12, &position, 12);
	pConstantBuffer->ResetBuffer(cbuf);
}
