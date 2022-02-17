#pragma once
#include "Math/Math.h"

class Texture
{
public:
	Texture();
	Texture(const char *path);
	~Texture();
	unsigned char *GetBufferPtr();
	int GetWidth() const;
	int GetHeight() const;
	int GetChannel() const;

	//Vec3f GetPixel(float u, float v);
	//Vec3f GetBilinearPixel(float u, float v);
private:
	int width;
	int height;
	int channels;

	unsigned char *buffer{};
};