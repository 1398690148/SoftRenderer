#include "Camera.h"

Camera::Camera(Vec3f e, Vec3f cen, Vec3f up, float aspect)
	: eye(e), center(cen), up(up), aspect(aspect)
{
}

Matrix Camera::GetMatrix()
{
	//Matrix pos = GetTranslateMatrix();// GetRotationMatrix();
	//const auto pos = dx::XMVector3Transform(
	//	dx::XMVectorSet(0.0f, 0.0f, -r, 0.0f),
	//	dx::XMMatrixRotationRollPitchYaw(phi, -theta, 0.0f)
	//);
	//return dx::XMMatrixLookAtLH(
	//	pos, dx::XMVectorZero(),
	//	dx::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)
	//) * dx::XMMatrixRotationRollPitchYaw(
	//	pitch, -yaw, roll
	//);
	Vec3f z = (eye - center).normalize();
	Vec3f x = cross(up, z).normalize();
	Vec3f y = cross(z, x).normalize();
	Matrix ModelView = Matrix::identity();
	for (int i = 0; i < 3; i++)
	{
		ModelView[0][i] = x[i];
		ModelView[1][i] = y[i];
		ModelView[2][i] = z[i];
		ModelView[i][3] = -center[i];
	}
	return ModelView;
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
