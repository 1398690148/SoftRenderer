#pragma once
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/geometric.hpp>

namespace Utils
{
	extern glm::vec3 BarycentricLerp(const glm::vec3 &v0, const glm::vec3 &v1, const glm::vec3 &v2, const glm::vec3 &w);

	extern glm::vec4 BarycentricLerp(const glm::vec4 &v0, const glm::vec4 &v1, const glm::vec4 &v2, const glm::vec3 &w);

	extern glm::vec3 Barycentric(glm::vec2 A, glm::vec2 B, glm::vec2 C, glm::vec2 P);

	extern glm::vec4 lerp(const glm::vec4 &A, const glm::vec4 &B, float t);
}

