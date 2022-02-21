#pragma once
#include "Bindable.h"

class Sampler : public Bindable
{
public:
	Sampler(Graphics &gfx, int idx);
	void Bind(Graphics &gfx);
private:
	SamplerState *pSampler{};
	int index;
};
