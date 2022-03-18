#include "TransformCbuf.h"

TransformCbuf::TransformCbuf(Graphics& gfx, const Drawable& parent)
	: vcbuf(gfx), parent(parent)
{}

void TransformCbuf::Bind(Graphics& gfx)
{
	glm::mat4 Model = parent.GetTransformXM();

	const auto modelView = gfx.GetCamera() * parent.GetTransformXM();
	const Transforms tf =
	{
		gfx.GetProjection() * modelView,
		Model,
	};
	glm::mat4 matrix = tf.modelViewProj;
	vcbuf.Update(gfx, tf);
	vcbuf.Bind(gfx);
}