#include "SRDevice.h"

SRDevice::SRDevice()
{
}

SRDevice::~SRDevice()
{
}

bool SRDevice::CreateBuffer(BUFFER_DESC *bd, SUBRESOURCE_DATA *sd, SRIBuffer **buffer)
{
	if (sd)
		*buffer = new SRIBuffer(bd->ByteWidth, bd->StructureByteStride, sd->pSysMem);
	else
		*buffer = new SRIBuffer(bd->ByteWidth, bd->StructureByteStride, true);
	return (*buffer != nullptr);
}

bool SRDevice::CreateRenderTargetView(int width, int height, SRRenderTargetView **pRTView)
{
	*pRTView = new SRRenderTargetView(width, height);
	return (*pRTView != nullptr);
}

bool SRDevice::CreateDepthStencilView(SRTexture2D *pDepthBuffer, DEPTH_STENCIL_VIEW_DESC *desc, SRDepthStencilView **ppDepthStencilView)
{
	*ppDepthStencilView = new SRDepthStencilView(desc->width, desc->height);
	return (*ppDepthStencilView != nullptr);
}

bool SRDevice::CreateInputLayout(const INPUT_ELEMENT_DESC *pInputElementDescs, unsigned int NumElements, SRInputLayout **ppInputLayout)
{
	*ppInputLayout = new SRInputLayout(pInputElementDescs, NumElements);
	return (*ppInputLayout != nullptr);
}

bool SRDevice::CreateTexture2D(TEXTURE2D_DESC *desc, SUBRESOURCE_DATA *sd, SRTexture2D **pTexture)
{
	if (!desc) return false;
	if (desc->BindFlags != BIND_SHADER_RESOURCE && desc->BindFlags != BIND_DEPTH_STENCIL) return false;
	*pTexture = sd ? new SRTexture2D(desc, sd) : new SRTexture2D(desc);
	return (*pTexture) != nullptr;
}

bool SRDevice::CreateSamplerState(SAMPLER_DESC *desc, SRSamplerState **pSamplerState)
{
	*pSamplerState = new SRSamplerState(desc);
	return (pSamplerState != nullptr);
}

bool SRDevice::CreateBlendState(BLEND_DESC *desc, SRBlendState ** pBlendState)
{
	*pBlendState = new SRBlendState(desc);
	return (*pBlendState != nullptr);
}
