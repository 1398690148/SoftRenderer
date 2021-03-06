#pragma once
#include <vector>
#include <Drawable.h>

class Graphics;

class Mesh : public Drawable
{
public:
	Mesh(Graphics& gfx, std::vector<std::shared_ptr<Bindable>> bindPtrs, glm::mat4 transform);
	void Draw(Graphics& gfx, glm::mat4 accumulatedTransform) const;
	void SetInitTransform(glm::mat4 transform);
	glm::mat4 GetInitTransform() const;
	glm::mat4 GetTransformXM() const override;
private:
	mutable glm::mat4 transform;
	glm::mat4 initTransform;
};

class Node
{
	friend class Model;
public:
	Node(const std::string& name, std::vector<Mesh*> meshPtrs, const glm::mat4& transform);
	void Draw(Graphics& gfx, glm::mat4 accumulatedTransform) const;
	void SetAppliedTransform(glm::mat4 transform);
private:
	void AddChild(std::unique_ptr<Node> pChild);
private:
	std::string name;
	std::vector<std::unique_ptr<Node>> childPtrs;
	std::vector<Mesh*> meshPtrs;
	glm::mat4 transform;
	glm::mat4 appliedTransform;
};