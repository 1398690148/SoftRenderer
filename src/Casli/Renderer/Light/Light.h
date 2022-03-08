#pragma once
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <RendererAPI.h>

class Graphics;
class PixelConstantBuffer;

class Light
{
public:
	virtual ~Light() {}
	virtual void Bind(Graphics &gfx) = 0;
protected:
	glm::vec3 color;
	PixelConstantBuffer *pConstantBuffer{};
	unsigned char *cbuf;
};