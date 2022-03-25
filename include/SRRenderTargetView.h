#pragma once
#include "SRCoreAPI.h"
#include <vector>
#include <array>
#include <glm/vec4.hpp>

class CORE_API SRRenderTargetView
{
public:
	SRRenderTargetView(unsigned int w, unsigned int h);
	void ClearBuffer(const glm::vec4 &ColorRGBA);
	void SetPixel(int i, int j, int k, unsigned char r, unsigned char g, unsigned char b, unsigned char a);
	std::array<glm::ivec4, 4> &GetPixel(int i, int j);
	std::array<glm::ivec4, 4> &GetPixel(int index);
private:
	unsigned int width;
	unsigned int height;
	std::vector<std::array<glm::ivec4, 4>> m_Buffer;
};