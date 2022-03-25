#pragma once
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtx/transform.hpp>

class Graphics;

class Light
{
public:
	virtual void Bind(Graphics &gfx, glm::mat4 view) = 0;
	virtual void Draw(Graphics& gfx) const = 0;
	void SetEyePos(glm::vec3 pos);
protected:
	glm::vec3 eye;
};