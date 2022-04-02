#include "SpotLight.h"

SpotLight::SpotLight(Graphics &gfx, int offset, glm::vec3 color, glm::vec3 position, glm::vec3 dir, glm::vec2 angleCoe, glm::vec3 attenuation)
	: Light(gfx, offset), cbuf(gfx)
{
	cbData.color = color;
	cbData.pos = position;
	cbData.dir = dir;
	cbData.cutOff = glm::cos(glm::radians(angleCoe.x));
	cbData.outerCutOff = glm::cos(glm::radians(angleCoe.y));
	cbData.constant = attenuation.x;
	cbData.linear = attenuation.y;
	cbData.quadratic = attenuation.z;
}

void SpotLight::Draw(Graphics& gfx) const
{
	mesh.SetPos(cbData.pos);
	mesh.Draw(gfx);
}

void SpotLight::Bind(Graphics& gfx, glm::mat4 view)
{
	auto dataCopy = cbData;
	const auto pos = cbData.pos;
	dataCopy.pos = view * glm::vec4(pos, 1.0);
	cbData.pos = dataCopy.pos;
	const auto dir = cbData.dir;
	dataCopy.dir = view * glm::vec4(dir, 1.0);
	cbData.dir = dataCopy.dir;
	cbuf.Update(gfx, offset, dataCopy);
	cbuf.Bind(gfx);
}