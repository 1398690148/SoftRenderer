#pragma once
#include "Bindable.h"

class Sampler : public Bindable
{
public:
	Sampler(Graphics &gfx, int idx, FILTER filter);
	void Bind(Graphics &gfx);
private:
	SRSamplerState *pSampler{};
	int index;
};
