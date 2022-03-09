#include "Sampler.h"

Sampler::Sampler(Graphics &gfx, int idx, FILTER filter) : index(idx)
{
	SAMPLER_DESC samplerDesc = {};
	samplerDesc.Filter = filter;
	samplerDesc.AddressU = TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = TEXTURE_ADDRESS_WRAP;

	if (!GetDevice(gfx)->CreateSamplerState(&samplerDesc, &pSampler))
	{
		throw;
	}
}

void Sampler::Bind(Graphics & gfx)
{
	GetContext(gfx)->PSSetSamplerState(index, &pSampler);
}
