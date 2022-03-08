#pragma once
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <vector>
#include <Bindable.h>
#include "Mesh.h"

class Mesh;
class Texture;
class VertexConstantBuffer;

class RENDERER_API Model
{
public:
	Model(Graphics &gfx, const char *path);
	void Draw(Graphics &gfx);
private:
	void loadModel(Graphics &gfx, std::string path);
	void processNode(Graphics &gfx, aiNode *node, const aiScene *scene);
	Mesh processMesh(Graphics &gfx, aiMesh *mesh, const aiScene *scene);
	void loadMaterialTextures(Graphics &gfx, aiMaterial *mat, aiTextureType type, std::string typeName, std::vector<Texture *> &textures);
private:
	std::vector<Mesh> meshes;
	std::string directory;
	VertexConstantBuffer *pConstantBuffer{};
	float z = 0;
};