#pragma once
#include "SRCoreAPI.h"
#include <vector>
#include <glm/vec4.hpp>

class CORE_API SRRenderTargetView
{
public:
	SRRenderTargetView(unsigned int w, unsigned int h);
	//SRRenderTargetView(unsigned int w, unsigned int h, unsigned int structureByteStride, void *gFbo);
	void ClearBuffer(const glm::vec4 &ColorRGBA);
	void SetPixel(int i, int j, int k, unsigned char r, unsigned char g, unsigned char b, unsigned char a);
	std::array<glm::i8vec4, 4> &GetPixel(int i, int j);
private:
	unsigned int width;
	unsigned int height;
	std::vector<std::array<glm::i8vec4, 4>> m_Buffer;
};