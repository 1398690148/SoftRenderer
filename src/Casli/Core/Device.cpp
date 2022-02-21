#include "Device.h"
#include "Texture2D.h"
#include "SamplerState.h"
#include "RenderTargetView.h"
#include "InputLayout.h"
#include "DepthStencilView.h"

Device::Device()
{
}

Device::~Device()
{
}

void Device::CreateBuffer(BUFFER_DESC *bd, SUBRESOURCE_DATA *sd, IBuffer **buffer)
{
	*buffer = new IBuffer(bd->ByteWidth, bd->StructureByteStride, sd->pSysMem);
}

void Device::CreateRenderTargetView(void *gFbo, int width, int height, RenderTargetView **pRTView)
{
	*pRTView = new RenderTargetView(width, height, 4, gFbo);
}

void Device::CreateDepthStencilView(Texture2D *pDepthBuffer, DEPTH_STENCIL_VIEW_DESC *desc, DepthStencilView **ppDepthStencilView)
{
	*ppDepthStencilView = new DepthStencilView(pDepthBuffer);
}

void Device::CreateInputLayout(const INPUT_ELEMENT_DESC * pInputElementDescs, unsigned int NumElements, InputLayout **ppInputLayout)
{
	*ppInputLayout = new InputLayout(pInputElementDescs, NumElements);
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
