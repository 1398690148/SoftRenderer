#pragma once
#include <VertexBuffer.h>
#include <IndexBuffer.h>
#include <Drawable.h>
#include <ConstantBuffers.h>
#include <glm/mat4x4.hpp>

class Graphics;

class Plane : public Drawable
{
public:
	Plane(Graphics &gfx, std::vector<std::shared_ptr<Bindable>> binds, glm::mat4 transform);
	glm::mat4 GetTransformXM() const;
	glm::mat4 GetInitTransform() const;
private:
	glm::mat4 transform;
	glm::mat4 initTransform;

};