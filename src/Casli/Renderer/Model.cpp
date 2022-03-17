#include "Model.h"
#include <Texture.h>
#include <IndexBuffer.h>
#include <VertexBuffer.h>
#include <Vertex.h>
#include <InputLayout.h>

Model::Model(Graphics &gfx, std::vector<std::shared_ptr<Bindable>> bindVec, const std::string fileName, glm::mat4 transform)
	: pGfx(gfx), binds(bindVec)
{
	Assimp::Importer imp;
	const auto pScene = imp.ReadFile(fileName.c_str(),
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_ConvertToLeftHanded |
		aiProcess_GenNormals
	);

	if (pScene == nullptr)
	{
		throw ;
	}
	directory = fileName.substr(0, fileName.find_last_of('/'));
	for (size_t i = 0; i < pScene->mNumMeshes; i++)
	{
		meshPtrs.push_back(ParseMesh(gfx, *pScene, *pScene->mMeshes[i]));
	}

	pRoot = ParseNode(*pScene->mRootNode);
	pRoot->SetAppliedTransform(transform);
}


void Model::Draw(Graphics& gfx, glm::mat4 transform) const
{
	pRoot->Draw(gfx, transform);
}

std::unique_ptr<Mesh> Model::ParseMesh(Graphics& gfx, const aiScene &pScene, const aiMesh& mesh)
{
	using Dvtx::VertexLayout;

	Dvtx::VertexBuffer vbuf(std::move(
		VertexLayout{}
		.Append(VertexLayout::Position3D)
		.Append(VertexLayout::Normal)
		.Append(VertexLayout::Texture2D)
	));

	for (unsigned int i = 0; i < mesh.mNumVertices; i++)
	{
		vbuf.EmplaceBack(
			*reinterpret_cast<glm::vec3*>(&mesh.mVertices[i]),
			*reinterpret_cast<glm::vec3*>(&mesh.mNormals[i]),
			*reinterpret_cast<glm::vec2*>(&mesh.mTextureCoords[0][i])
		);
	}

	std::vector<unsigned int> indices;
	indices.reserve(mesh.mNumFaces * 3);
	for (unsigned int i = 0; i < mesh.mNumFaces; i++)
	{
		const auto& face = mesh.mFaces[i];
		assert(face.mNumIndices == 3);
		indices.push_back(face.mIndices[0]);
		indices.push_back(face.mIndices[1]);
		indices.push_back(face.mIndices[2]);
	}
	if (mesh.mMaterialIndex >= 0)
	{
		aiMaterial *material = pScene.mMaterials[mesh.mMaterialIndex];
		loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse", 0);
		loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular", 1);
		loadMaterialTextures(material, aiTextureType_NORMALS, "texture_normal", 2);
	}
	binds.push_back(std::make_shared<VertexBuffer>(gfx, vbuf));
	binds.push_back(std::make_shared<IndexBuffer>(gfx, indices));
	binds.push_back(std::make_shared<InputLayout>(gfx, vbuf.GetLayout().GetD3DLayout()));

	return std::make_unique<Mesh>(gfx, std::move(binds));
}

void Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName, int idx)
{
	if (!mat->GetTextureCount(type))
	{
		return;
	}
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		std::string path = directory + "/" + str.C_Str();
		binds.push_back(std::make_shared<Texture>(pGfx, path.c_str(), 10, idx));
	}
}

std::unique_ptr<Node> Model::ParseNode(const aiNode& node)
{
	glm::mat4 transform = glm::mat4(1.0);
	Tools::CopyaiMat(&node.mTransformation, transform);

	std::vector<Mesh*> curMeshPtrs;
	curMeshPtrs.reserve(node.mNumMeshes);
	for (size_t i = 0; i < node.mNumMeshes; i++)
	{
		const auto meshIdx = node.mMeshes[i];
		curMeshPtrs.push_back(meshPtrs.at(meshIdx).get());
	}

	auto pNode = std::make_unique<Node>(node.mName.C_Str(), std::move(curMeshPtrs), transform);
	for (size_t i = 0; i < node.mNumChildren; i++)
	{
		pNode->AddChild(ParseNode(*node.mChildren[i]));
	}

	return pNode;
}
