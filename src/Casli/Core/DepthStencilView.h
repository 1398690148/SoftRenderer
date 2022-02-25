#pragma once
#include "CoreAPI.h"
#include "Texture2D.h"

class CORE_API DepthStencilView : public IBuffer
{
public:
	DepthStencilView(Texture2D *buffer);
	void ClearBuffer(const float depth);
	float GetDepth(int i, int j);
	void SetDepth(int i, int j, float depth);
private:
	unsigned int width;
	unsigned int height;
	unsigned int maxHeight;
};