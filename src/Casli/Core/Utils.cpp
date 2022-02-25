#include "Utils.h"

glm::vec3 BarycentricLerp(const glm::vec3 &v0, const glm::vec3 &v1, const glm::vec3 &v2, const glm::vec3 &w)
{
	return w[0] * v0 + w[1] * v1 + w[2] * v2;
}

glm::vec3 Barycentric(glm::vec2 A, glm::vec2 B, glm::vec2 C, glm::vec2 P)
{
	glm::vec3 s[2];
	for (int i = 1; i >= 0; i--)
	{
		s[i][0] = C[i] - A[i];
		s[i][1] = B[i] - A[i];
		s[i][2] = A[i] - P[i];
	}
	glm::vec3 u = glm::cross(s[0], s[1]);
	if (std::abs(u[2]) > 1e-2)
		return glm::vec3(1.f - (u.x + u.y) / u.z, u.y / u.z, u.x / u.z);
	return glm::vec3(-1, 1, 1);
}
