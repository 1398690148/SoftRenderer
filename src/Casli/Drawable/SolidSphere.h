#pragma once
#include <Drawable.h>

class SolidSphere : public Drawable
{
public:
	SolidSphere(Graphics& gfx, float radius, glm::vec3 color);
	void SetPos(glm::vec3 pos);
	glm::mat4 GetTransformXM() const override;
	glm::mat4 GetInitTransform() const;
private:
	glm::vec3 pos = { 1.0f,1.0f,1.0f };
	glm::mat4 initTransform;
};