#pragma once
#include "SRCoreAPI.h"
#include "SRTexture2D.h"
#include "SRIBuffer.h"

class CORE_API SRDepthStencilView : public SRIBuffer
{
public:
	SRDepthStencilView(SRTexture2D *buffer);
	void ClearBuffer(const float depth);
	float GetDepth(int i, int j);
	void SetDepth(int i, int j, float depth);
private:
	unsigned int width;
	unsigned int height;
	unsigned int maxHeight;
};