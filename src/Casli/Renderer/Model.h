#pragma once
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <vector>
#include <Drawable.h>
#include <Mesh.h>

class Texture;
class VertexConstantBuffer;
class Sampler;

class RENDERER_API Model : public Drawable
{
public:
	Model(Graphics &gfx, const char *path, int mipLevel = 1, FILTER filter = FILTER_POINT_MIP_POINT);
	void Draw();
	void Bind(unsigned char *ConstantBuffer, size_t size) override;
private:
	void loadModel(std::string path);
	void processNode(aiNode *node, const aiScene *scene);
	Mesh processMesh(aiMesh *mesh, const aiScene *scene);
	void loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName, std::vector<Texture *> &textures, int idx);
private:
	std::vector<Mesh> meshes;
	std::string directory;
	VertexConstantBuffer *pConstantBuffer{};
	Sampler *sampler{};
};