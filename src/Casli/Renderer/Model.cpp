#include "Model.h"
#include <Texture.h>
#include <Sampler.h>
#include <glm/gtc/matrix_transform.hpp>
#include <VertexConstantBuffer.h>

Model::Model(Graphics &gfx, const char *path, int mipLevel, FILTER filter) : Drawable(gfx)
{
	loadModel(path);
	std::vector<glm::mat4> matrix;
	matrix.push_back(glm::mat4(1.0));
	matrix.push_back(glm::mat4(1.0));
	matrix.push_back(glm::mat4(1.0));
	pConstantBuffer = new VertexConstantBuffer(gfx, 0, (unsigned char*)&matrix[0], sizeof(glm::mat4) * matrix.size());
	sampler = new Sampler(gfx, 0, filter);
}

void Model::Draw()
{
	for (auto &mesh : meshes)
	{
		mesh.Draw(pGfx);
	}
}

void Model::Bind(unsigned char *ConstantBuffer, size_t size)
{
	GetContext(pGfx)->SetRenderState(state);
	pConstantBuffer->SetConstantBuffer(ConstantBuffer, size);
	pConstantBuffer->Bind(pGfx);
	sampler->Bind(pGfx);
}

void Model::loadModel(std::string path)
{
	Assimp::Importer importer;
	const aiScene *scene = importer.ReadFile(path, aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_SortByPType);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		return;
	}
	directory = path.substr(0, path.find_last_of('/'));
	processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode * node, const aiScene * scene)
{
	for (int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(mesh, scene));
	}
	for (int i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene);
	}
}

Mesh Model::processMesh(aiMesh * mesh, const aiScene * scene)
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture *> textures;

	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		glm::vec3 vector;
		//顶点
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.Position = vector;
		//法线
		if (mesh->mNormals)
		{
			vector.x = mesh->mNormals[i].x;
			vector.y = mesh->mNormals[i].y;
			vector.z = mesh->mNormals[i].z;
			vertex.Normal = vector;
		}
		else
			vertex.Normal = glm::vec3(0.0f, 0.0f, 0.0f);
		//纹理坐标
		if (mesh->mTextureCoords[0]) // 网格是否有纹理坐标？
		{
			glm::vec2 vec;
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.TexCoords = vec;
		}
		else
			vertex.TexCoords = glm::vec2(0.0f, 0.0f);
		vertices.push_back(vertex);
	}
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}
	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
		loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse", textures, 0);
		loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular", textures, 1);
		loadMaterialTextures(material, aiTextureType_NORMALS, "texture_normal", textures, 2);
	}
	return Mesh(pGfx, vertices, indices, textures);
}

void Model::loadMaterialTextures(aiMaterial * mat, aiTextureType type, std::string typeName, std::vector<Texture *> &textures, int idx)
{
	if (!mat->GetTextureCount(type))
	{
		textures.push_back(nullptr);
		return;
	}
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		std::string path = directory + "/" + str.C_Str();
		Texture *texture = new Texture(pGfx, path.c_str(), 10, idx);
		textures.push_back(texture);
	}
}
