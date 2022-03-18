#pragma once
#include <Light.h>
#include <SolidSphere.h>
#include <ConstantBuffers.h>

class PointLight : public Light
{
public:
	PointLight(Graphics &gfx, glm::vec3 color, glm::vec3 position, float constant, float linear, float exp);
	void Draw(Graphics& gfx) const;
	void Bind(Graphics& gfx, glm::mat4 view);

private:
	struct PointLightCBuf
	{
		glm::vec3 pos;
		glm::vec3 color;
		float Constant;
		float Linear;
		float Exp;
	};
protected:
	PointLightCBuf cbData;
	mutable SolidSphere mesh;
	mutable PixelConstantBuffer<PointLightCBuf> cbuf;
};