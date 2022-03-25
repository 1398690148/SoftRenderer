#include "PointLight.h"

PointLight::PointLight(Graphics &gfx, glm::vec3 color, glm::vec3 position, float constant, float linear, float exp)
	: mesh(gfx, 5), cbuf(gfx)
{
	cbData.color = color;
	cbData.pos = position;
	cbData.Constant = constant;
	cbData.Linear = linear;
	cbData.Exp = exp;
}

void PointLight::Draw(Graphics& gfx) const
{
	mesh.SetPos(cbData.pos);
	mesh.Draw(gfx);
}

void PointLight::Bind(Graphics& gfx, glm::mat4 view)
{
	auto dataCopy = cbData;
	const auto pos = cbData.pos;
	dataCopy.pos = view * glm::vec4(pos, 1.0);
	cbData.pos = dataCopy.pos;
	cbuf.Update(gfx, dataCopy);
	cbuf.Bind(gfx);
}