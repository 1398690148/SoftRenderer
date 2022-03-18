#include "Mesh.h"
#include "TransformCbuf.h"
#include <VertexBuffer.h>
#include <IndexBuffer.h>

// Mesh
Mesh::Mesh(Graphics& gfx, std::vector<std::shared_ptr<Bindable>> bindPtrs)
{
	transform = glm::mat4(1.0);
	for (auto& pb : bindPtrs)
	{
		AddBind(std::move(pb));
	}
	AddBind(std::make_unique<TransformCbuf>(gfx, *this));
}
void Mesh::Draw(Graphics& gfx, glm::mat4 accumulatedTransform) const
{
	transform = accumulatedTransform;
	Drawable::Draw(gfx);
}
glm::mat4 Mesh::GetTransformXM() const
{
	return transform;
}

// Node
Node::Node(const std::string& name, std::vector<Mesh*> meshPtrs, const glm::mat4& transform_in)
	: meshPtrs(std::move(meshPtrs)), name(name)
{
	transform = transform_in;
	appliedTransform = glm::mat4(1.0);
}

void Node::Draw(Graphics& gfx, glm::mat4 accumulatedTransform) const
{
	const auto built = appliedTransform  * transform * accumulatedTransform;
	for (const auto pm : meshPtrs)
	{
		pm->Draw(gfx, built);
	}
	for (const auto& pc : childPtrs)
	{
		pc->Draw(gfx, built);
	}
}

void Node::AddChild(std::unique_ptr<Node> pChild)
{
	assert(pChild);
	childPtrs.push_back(std::move(pChild));
}

void Node::SetAppliedTransform(glm::mat4 transform)
{
	appliedTransform = transform;
}
