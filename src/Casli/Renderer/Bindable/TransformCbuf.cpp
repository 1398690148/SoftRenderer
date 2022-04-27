#include "TransformCbuf.h"

TransformCbuf::TransformCbuf(Graphics& gfx, const Drawable& parent)
	: vcbuf(gfx), parent(parent)
{}

void TransformCbuf::Bind(Graphics& gfx)
{
	glm::mat4 Model = parent.GetInitTransform() * parent.GetTransformXM();
	glm::mat4 ModelT = glm::transpose(Model);

	const auto modelView = gfx.GetCamera() * Model;
	const Transforms tf =
	{
		gfx.GetProjection() * modelView,
		Model,
		ModelT,
	};
	glm::mat4 matrix = tf.modelViewProj;
	vcbuf.Update(gfx, 0, tf);
	vcbuf.Bind(gfx);
}