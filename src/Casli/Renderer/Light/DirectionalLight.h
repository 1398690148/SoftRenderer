#pragma once
#include <Light.h>
#include <SolidSphere.h>
#include <ConstantBuffers.h>

class DirectionalLight : public Light
{
public:
	DirectionalLight(Graphics &gfx, glm::vec3 color, glm::vec3 dir);
	void Draw(Graphics& gfx) const;
	void Bind(Graphics& gfx, glm::mat4 view);

private:
	struct DirectionalLightCbuf
	{
		glm::vec3 eye;
		glm::vec3 color;
		glm::vec3 dir;
	};
protected:
	DirectionalLightCbuf cbData;
	mutable SolidSphere mesh;
	mutable PixelConstantBuffer<DirectionalLightCbuf> cbuf;
};