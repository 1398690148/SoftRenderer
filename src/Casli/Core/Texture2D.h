#pragma once
#include "CoreAPI.h"
#include "IBuffer.h"

class CORE_API Texture2D : public IBuffer
{
public:
	Texture2D(TEXTURE2D_DESC *desc, SUBRESOURCE_DATA *sd);
	~Texture2D();

	void GenerateMips();
private:
	unsigned int width;
	unsigned int height;
	unsigned int mipLevels;
	DataFormat format;
};