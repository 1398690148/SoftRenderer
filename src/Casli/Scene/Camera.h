#pragma once
#include "Graphics.h"

class Camera
{
public:
	Camera(Vec3f e, Vec3f cen, Vec3f up, float aspect);
	Matrix GetMatrix();
	//void SpawnControlWindow();
	//void Reset();
private:
	//Matrix GetTranslateMatrix();
	//Matrix GetRotationMatrix();
private:
	Vec3f eye;
	Vec3f center;
	Vec3f up;
	float aspect;
};