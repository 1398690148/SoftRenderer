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

//Matrix Camera::GetTranslateMatrix()
//{
//	Matrix matrix = Matrix::identity();
//	matrix[2][3] = -r;
//	return matrix;
//}
//
//Matrix Camera::GetRotationMatrix()
//{
//	Matrix matrix = Matrix::identity();
//	float cosp = cos(phi);
//	float sinp = sin(phi);
//	float cosy = sin(yaw);
//	float siny = sin(yaw);
//	float cosr = sin(roll);
//	float sinr = sin(roll);
//	matrix[0][0] = cosp * cosr - cosy * sinp * sinr;
//	matrix[0][1] = -cosy * cosr * sinp - cosp * sinr;
//	matrix[0][2] = sinp * siny;
//	matrix[1][0] = cosr * sinp + cosp * cosy * sinr;
//	matrix[1][1] = cosp * cosy * cosr - sinp * sinr;
//	matrix[1][2] = -cosp * siny;
//	matrix[2][0] = siny * sinr;
//	matrix[2][1] = cosr * siny;
//	matrix[2][2] = cosy;
//	return matrix;
//}
