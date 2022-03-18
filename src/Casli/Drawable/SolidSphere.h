#pragma once
#include <Drawable.h>

class SolidSphere : public Drawable
{
public:
	SolidSphere(Graphics& gfx, float radius);
	void Update(float dt);
	void SetPos(glm::vec3 pos);
	glm::mat4 GetTransformXM() const override;
private:
	glm::vec3 pos = { 1.0f,1.0f,1.0f };
};