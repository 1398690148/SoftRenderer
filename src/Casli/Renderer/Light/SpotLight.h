#pragma once
#include <Light.h>
#include <SolidSphere.h>
#include <ConstantBuffers.h>

class SpotLight : public Light
{
public:
	SpotLight(Graphics &gfx, glm::vec3 color, glm::vec3 position, glm::vec3 dir, glm::vec2 angleCoe, glm::vec3 attenuation);
	void Draw(Graphics& gfx) const;
	void Bind(Graphics& gfx, glm::mat4 view);

private:
	struct SpotLightCBuf
	{
		glm::vec3 pos;
		glm::vec3 color;
		glm::vec3 dir;
		float cutOff;
		float outerCutOff;
		float constant;
		float linear;
		float quadratic;
	};
protected:
	SpotLightCBuf cbData;
	mutable SolidSphere mesh;
	mutable PixelConstantBuffer<SpotLightCBuf> cbuf;
};