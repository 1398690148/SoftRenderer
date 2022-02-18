#pragma once
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <vector>
#include "Mesh.h"

class Model
{
public:
	Model(const char *path);
private:
	std::vector<Mesh> meshes;
};