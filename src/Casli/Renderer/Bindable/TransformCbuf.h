#pragma once
#include "ConstantBuffers.h"
#include "Drawable.h"

class TransformCbuf : public Bindable
{
private:
	struct Transforms
	{
		glm::mat4 modelViewProj;
		glm::mat4 model;
	};
public:
	TransformCbuf(Graphics& gfx, const Drawable& parent);
	void Bind(Graphics& gfx) override;
private:
	VertexConstantBuffer<Transforms> vcbuf;
	const Drawable& parent;
};