#pragma once
#include "CoreAPI.h"
#include "IBuffer.h"

class CORE_API RenderTargetView : public IBuffer
{
public:
	RenderTargetView(unsigned int w, unsigned int h, unsigned int structureByteStride, const void *buffer);
	void ClearBuffer(const float ColorRGBA[4]);
	void SetPixel(int i, int j, unsigned char r, unsigned char g, unsigned char b, unsigned char a);
private:
	unsigned int width;
	unsigned int height;
};