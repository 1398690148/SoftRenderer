#pragma once
#include "SRCoreAPI.h"
#include "SRTexture2D.h"
#include "SRSamplerState.h"
#include <string>
#include <vector>
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
