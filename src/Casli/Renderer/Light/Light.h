#pragma once
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

class Graphics;

class Light
{
public:
	virtual ~Light() {}
	virtual void Bind(Graphics &gfx) = 0;
	virtual void rotate(float angle, glm::vec3 axis);
protected:
	glm::vec3 color;
	unsigned char *cbuf;
};