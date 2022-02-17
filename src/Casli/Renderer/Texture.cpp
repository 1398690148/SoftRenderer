#include <algorithm>
#include "Texture.h"
#include "stb_image/stb_image.cpp"

Texture::Texture()
{
}

Texture::Texture(const char * path)
{
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(path, &width, &height, &channels, 0);
	if (data != nullptr)
	{
		int size = width * height * channels;
		buffer = new unsigned char[size];
		for (int i = 0; i < size; i++)
		{
			buffer[i] = data[i];
		}
	}
	stbi_image_free(data);
}

Texture::~Texture()
{

}

unsigned char *Texture::GetBufferPtr()
{
	return buffer;
}

int Texture::GetWidth() const
{
	return width;
}

int Texture::GetHeight() const
{
	return height;
}

int Texture::GetChannel() const
{
	return channels;
}

//Vec3f Texture::GetPixel(float u, float v)
//{
//	if (u > 1 || v > 1 || u < 0 || v < 0)
//	{
//		return Vec3f(255, 255, 255);
//	}
//	int x = u * (width - 1);
//	int y = v * (height - 1);
//
//	int index = (x + y * width) * channels;
//	int r = pixels[index];
//	int g = pixels[index + 1];
//	int b = pixels[index + 2];
//	return Vec3f(r, g, b);
//}
//
//Vec3f Texture::GetBilinearPixel(float u, float v)
//{
//	if (u > 1 || v > 1 || u < 0 || v < 0)
//	{
//		return Vec3f(255, 255, 255);
//	}
//	int w = width - 1;
//	int h = height - 1;
//	float ox = u * w;
//	float oy = v * h;
//	int x = u * w;
//	int y = v * h;
//
//	int index = (x + y * width) * channels;
//
//	int bottomR = pixels[index];
//	int bottomG = pixels[index + 1];
//	int bottomB = pixels[index + 2];
//
//	int leftUpR = bottomR;
//	int leftUpG = bottomG;
//	int leftUpB = bottomB;
//
//	if (y != h)
//	{
//		leftUpR = pixels[index + width * channels];
//		leftUpG = pixels[index + width * channels + 1];
//		leftUpB = pixels[index + width * channels + 2];
//	}
//
//	int rightUpR = bottomR;
//	int rightUpG = bottomG;
//	int rightUpB = bottomB;
//	if (x != w && y != h)
//	{
//		rightUpR = pixels[index + w * channels];
//		rightUpG = pixels[index + w * channels + 1];
//		rightUpB = pixels[index + w * channels + 2];
//	}
//
//
//	int rightBottomR = bottomR;
//	int rightBottomG = bottomG;
//	int rightBottomB = bottomB;
//	if (x != w)
//	{
//		rightBottomR = pixels[index + channels];
//		rightBottomG = pixels[index + channels + 1];
//		rightBottomB = pixels[index + channels + 1];
//	}
//
//
//	float midUpR = ((float)floor(ox + 1) - ox) * leftUpR + (ox - (float)floor(ox)) * rightUpR;
//	float midUpG = ((float)floor(ox + 1) - ox) * leftUpG + (ox - (float)floor(ox)) * rightUpG;
//	float midUpB = ((float)floor(ox + 1) - ox) * leftUpB + (ox - (float)floor(ox)) * rightUpB;
//
//	float midBottomR = ((float)floor(ox + 1) - ox) * bottomR + (ox - (float)floor(ox)) * rightBottomR;
//	float midBottomG = ((float)floor(ox + 1) - ox) * bottomG + (ox - (float)floor(ox)) * rightBottomG;
//	float midBottomB = ((float)floor(ox + 1) - ox) * bottomB + (ox - (float)floor(ox)) * rightBottomB;
//
//	float r = ((float)floor(oy + 1) - oy) * midBottomR + (oy - (float)floor(oy)) * midUpR;
//	float g = ((float)floor(oy + 1) - oy) * midBottomG + (oy - (float)floor(oy)) * midUpG;
//	float b = ((float)floor(oy + 1) - oy) * midBottomB + (oy - (float)floor(oy)) * midUpB;
//
//	return Vec3f(r, g, b);
//}