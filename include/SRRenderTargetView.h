#pragma once
#include "SRCoreAPI.h"
#include "SRIBuffer.h"
#include <glm/vec4.hpp>

class CORE_API SRRenderTargetView : public SRIBuffer
{
public:
	SRRenderTargetView(unsigned int w, unsigned int h, unsigned int structureByteStride);
	SRRenderTargetView(unsigned int w, unsigned int h, unsigned int structureByteStride, void *gFbo);
	void ClearBuffer(const glm::vec4 &ColorRGBA);
	void SetPixel(int i, int j, unsigned char r, unsigned char g, unsigned char b, unsigned char a);
	glm::vec4 GetPixel(int i, int j);
private:
	unsigned int width;
	unsigned int height;
};