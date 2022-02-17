#pragma once
#include "CoreAPI.h"
#include "Math/Math.h"
#include "Texture2D.h"
#include "Sampler.h"

struct Description
{
	std::string Name;
	unsigned int typeSize;
	unsigned int Size;
	unsigned int Offset;
};

class CORE_API IVertexShader
{
public:
	virtual unsigned char *vertex(unsigned char *input) = 0;
	virtual ~IVertexShader() { delete buffer; }
	unsigned char *buffer{};
	std::vector<Description> inDesc;
	std::vector<Description> outDesc;
	std::vector<Texture2D> textures;
	std::vector<Sampler> samplers;
};