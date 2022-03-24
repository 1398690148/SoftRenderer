#include "DirectionalLight.h"
#include <Graphics.h>
#include <glm/gtx/transform.hpp>

DirectionalLight::DirectionalLight(Graphics &gfx, glm::vec3 color, glm::vec3 dir)
	: mesh(gfx, 5), cbuf(gfx)
{
	cbData.eye = glm::vec3(0, 0, -2.3);
	cbData.color = color;
	cbData.dir = dir;

}

void DirectionalLight::Draw(Graphics& gfx) const
{
	//mesh.SetPos(cbData.pos);
	mesh.Draw(gfx);
}

void DirectionalLight::Bind(Graphics& gfx, glm::mat4 view)
{
	auto dataCopy = cbData;
	const auto dir = cbData.dir;
	dataCopy.dir = view * glm::vec4(dir, 1.0);
	cbData.dir = dataCopy.dir;

	cbData.eye = eye;
	dataCopy.eye = eye;

	cbuf.Update(gfx, dataCopy);
	cbuf.Bind(gfx);
}