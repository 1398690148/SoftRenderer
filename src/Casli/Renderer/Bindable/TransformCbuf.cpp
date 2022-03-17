#include "TransformCbuf.h"

TransformCbuf::TransformCbuf(Graphics& gfx, const Drawable& parent)
	: vcbuf(gfx), parent(parent)
{}

void TransformCbuf::Bind(Graphics& gfx) noexcept
{
	glm::mat4 Model = parent.GetTransformXM();
	vcbuf.Update(gfx, gfx.GetProjection() * gfx.GetCamera() * Model);
	vcbuf.Bind(gfx);
}