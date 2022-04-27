#pragma once
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <Tools.h>
#include <vector>
#include <Mesh.h>
#include <map>

class Texture;
class Sampler;

class Model
{
public:
	Model(Graphics &gfx, std::vector<std::shared_ptr<Bindable>> bindVec, const std::string fileName, glm::mat4 trans);
	void Draw(Graphics& gfx, glm::mat4 transform) const;
private:
	std::unique_ptr<Mesh> ParseMesh(Graphics& gfx, const aiMesh& mesh, const aiMaterial* const* pMaterials);
	std::unique_ptr<Node> ParseNode(const aiNode& node);
private:
	std::unique_ptr<Node> pRoot;
	std::vector<std::unique_ptr<Mesh>> meshPtrs;
	std::vector<std::shared_ptr<Bindable>> binds;
	std::map<std::string, std::shared_ptr<Texture>> textures;
	std::string directory;
	Graphics &pGfx;
	glm::mat4 initTransform;
};