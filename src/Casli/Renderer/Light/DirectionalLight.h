#pragma once
#include <Light.h>
#include <SolidSphere.h>
#include <ConstantBuffers.h>

class DirectionalLight : public Light
{
public:
	DirectionalLight(Graphics &gfx, int offset, glm::vec3 color, glm::vec3 dir);
	void Draw(Graphics& gfx) const;
	void Bind(Graphics& gfx, glm::mat4 view);

private:
	struct DirectionalLightCBuf
	{
		glm::vec3 eye;
		glm::vec3 color;
		glm::vec3 dir;
	};
protected:
	DirectionalLightCBuf cbData;
	mutable PixelConstantBuffer<DirectionalLightCBuf> cbuf;
};