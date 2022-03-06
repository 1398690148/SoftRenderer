#include "Device.h"
#include "Texture2D.h"
#include "SamplerState.h"
#include "RenderTargetView.h"
#include "InputLayout.h"
#include "DepthStencilView.h"
#include "BlendState.h"

Device::Device()
{
}

Device::~Device()
{
}

bool Device::CreateBuffer(BUFFER_DESC *bd, SUBRESOURCE_DATA *sd, IBuffer **buffer)
{
	*buffer = new IBuffer(bd->ByteWidth, bd->StructureByteStride, sd->pSysMem);
	return (*buffer != nullptr);
}

bool Device::CreateRenderTargetView(void *gFbo, int width, int height, RenderTargetView **pRTView)
{
	*pRTView = new RenderTargetView(width, height, 4, gFbo);
	return (*pRTView != nullptr);
}

bool Device::CreateDepthStencilView(Texture2D *pDepthBuffer, DEPTH_STENCIL_VIEW_DESC *desc, DepthStencilView **ppDepthStencilView)
{
	*ppDepthStencilView = new DepthStencilView(pDepthBuffer);
	return (*ppDepthStencilView != nullptr);
}

bool Device::CreateInputLayout(const INPUT_ELEMENT_DESC * pInputElementDescs, unsigned int NumElements, InputLayout **ppInputLayout)
{
	*ppInputLayout = new InputLayout(pInputElementDescs, NumElements);
	return (*ppInputLayout != nullptr);
}

bool Device::CreateTexture2D(TEXTURE2D_DESC *desc, SUBRESOURCE_DATA *sd, Texture2D **pTexture)
{
	if (!desc) return false;
	if (desc->BindFlags != BIND_SHADER_RESOURCE && desc->BindFlags != BIND_DEPTH_STENCIL) return false;
	*pTexture = new Texture2D(desc, sd);
	return (*pTexture) != nullptr;
}

bool Device::CreateSamplerState(SAMPLER_DESC *desc, SamplerState **pSamplerState)
{
	*pSamplerState = new SamplerState(desc);
	return (pSamplerState != nullptr);
}

bool Device::CreateBlendState(BLEND_DESC *desc, BlendState ** pBlendState)
{
	*pBlendState = new BlendState(desc);
	return (*pBlendState != nullptr);
}
