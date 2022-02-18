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

class CORE_API IPixelShader
{
public:
	IPixelShader();
	virtual bool fragment(unsigned char *in, glm::vec4 &color) = 0;
	virtual ~IPixelShader();
	unsigned char *buffer{};
	Texture2D **textures{};
	SamplerState **samplers{};
};