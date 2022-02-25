#include "Model.h"
#include <assimp/types.h>
#include "../Renderer/Texture.h"
#include <glm/gtc/matrix_transform.hpp>
#include "../Renderer/VertexConstantBuffer.h"

Model::Model(Graphics &gfx, const char *path)
{
	loadModel(gfx, path);
	std::vector<glm::mat4> matrix;
	matrix.push_back(glm::mat4(1.0));
	matrix.push_back(glm::mat4(1.0));
	matrix.push_back(glm::mat4(1.0));
	pConstantBuffer = new VertexConstantBuffer(gfx, 0, (unsigned char*)&matrix[0], sizeof(glm::mat4) * matrix.size());
}

void Model::Draw(Graphics &gfx)
{
	std::vector<glm::mat4> matrix;
	glm::mat4 MVP = glm::mat4(1.0);
	glm::mat4 MT = glm::mat4(1.0);
	glm::mat4 M = glm::mat4(1.0);
	MVP = glm::translate(MVP, glm::vec3(0, -12.f, -7));
	MVP = glm::rotate(MVP, glm::radians(z), glm::vec3(0, 1.f, 0));

	M = glm::translate(M, glm::vec3(0, -12.f, -7));
	M = glm::rotate(M, glm::radians(z), glm::vec3(0, 1.f, 0));

	MT = glm::translate(MT, glm::vec3(0, -12.f, -7));
	MT = glm::transpose(glm::inverse(glm::rotate(MT, glm::radians(z++), glm::vec3(0, 1.f, 0))));

	MVP = gfx.GetProjection() * gfx.GetCamera() * MVP;
	matrix.push_back(MVP);
	matrix.push_back(M);
	matrix.push_back(MT);
	pConstantBuffer->SetMatrix(matrix);
	pConstantBuffer->Bind(gfx);
	for (auto &mesh : meshes)
	{
		mesh.Draw(gfx);
		//meshes[0].Draw(gfx);
		//meshes[1].Draw(gfx);
		//meshes[2].Draw(gfx);
		//meshes[3].Draw(gfx);
		//meshes[4].Draw(gfx);
		//meshes[6].Draw(gfx);
		//meshes[5].Draw(gfx);
	}
}

void Model::loadModel(Graphics &gfx, std::string path)
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
	processNode(gfx, scene->mRootNode, scene);
}

void Model::processNode(Graphics &gfx, aiNode * node, const aiScene * scene)
{
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(gfx, mesh, scene));
	}
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		processNode(gfx, node->mChildren[i], scene);
	}
}

Mesh Model::processMesh(Graphics &gfx, aiMesh * mesh, const aiScene * scene)
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
		vector.x = mesh->mNormals[i].x;
		vector.y = mesh->mNormals[i].y;
		vector.z = mesh->mNormals[i].z;
		vertex.Normal = vector;
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
		loadMaterialTextures(gfx, material, aiTextureType_DIFFUSE, "texture_diffuse", textures);
		loadMaterialTextures(gfx, material, aiTextureType_SPECULAR, "texture_specular", textures);
	}
	return Mesh(gfx, vertices, indices, textures);
}

void Model::loadMaterialTextures(Graphics &gfx, aiMaterial * mat, aiTextureType type, std::string typeName, std::vector<Texture *> &textures)
{
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		std::string path = directory + "/" + str.C_Str();
		Texture *texture = new Texture(gfx, path.c_str(), 10, textures.size());
		textures.push_back(texture);
	}
}
