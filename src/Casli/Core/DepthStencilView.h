#pragma once
#include "CoreAPI.h"
#include "Texture2D.h"

class CORE_API DepthStencilView : public IBuffer
{
public:
	DepthStencilView(Texture2D *buffer);
	void ClearBuffer(const unsigned char depth);
	unsigned char GetDepth(int i, int j);
	void SetDepth(int i, int j, unsigned char depth);
private:
	unsigned int width;
	unsigned int height;
};