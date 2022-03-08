#include "Sampler.h"

Sampler::Sampler(Graphics &gfx, int idx) : index(idx)
{
	SAMPLER_DESC samplerDesc = {};
	samplerDesc.Filter = FILTER_POINT_MIP_POINT;
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
