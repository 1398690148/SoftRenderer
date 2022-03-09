#pragma once
#include "Bindable.h"

class RENDERER_API Sampler : public Bindable
{
public:
	Sampler(Graphics &gfx, int idx, FILTER filter);
	void Bind(Graphics &gfx);
private:
	SamplerState *pSampler{};
	int index;
};
