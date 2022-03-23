#pragma once
#include "SRCoreAPI.h"
#include <array>
#include <vector>

class CORE_API SRDepthStencilView
{
public:
	SRDepthStencilView(unsigned int w, unsigned int h);
	void ClearBuffer(const float depth);
	std::array<float, 4> &GetDepth(int i, int j);
	void SetDepth(int i, int j, int k, float depth);
private:
	unsigned int width;
	unsigned int height;
	std::vector<std::array<float, 4>> m_Buffer;
};