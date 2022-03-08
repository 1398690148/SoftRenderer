#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(glm::vec3 e, glm::vec3 cen, glm::vec3 up, float aspect)
	: eye(e), center(cen), up(up), aspect(aspect)
{
}

glm::mat4x4 Camera::GetMatrix()
{
	return glm::lookAt(eye, center, up);
	//glm::vec3 z = glm::normalize(eye - center);
	//glm::vec3 x = glm::normalize(cross(up, z));
	//glm::vec3 y = glm::normalize(cross(z, x));
	//glm::mat4x4 ModelView = glm::mat4x4(1.0);
	//for (int i = 0; i < 3; i++)
	//{
	//	ModelView[0][i] = x[i];
	//	ModelView[1][i] = y[i];
	//	ModelView[2][i] = z[i];
	//	ModelView[i][3] = -center[i];
	//}
	//return ModelView;
}

void Camera::Translate(glm::vec3 offset)
{
	eye += offset;
}
