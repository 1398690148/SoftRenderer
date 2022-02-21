#pragma once
#include "CoreAPI.h"
#include "Texture2D.h"
#include "SamplerState.h"
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat2x2.hpp>
#include <glm/mat2x3.hpp>
#include <glm/mat3x2.hpp>
#include <glm/mat3x3.hpp>
#include <glm/mat4x2.hpp>
#include <glm/mat4x3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/mat2x4.hpp>
#include <glm/mat3x4.hpp>

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
	std::vector<SamplerState> samplers;
};