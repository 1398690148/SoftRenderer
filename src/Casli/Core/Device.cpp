#include "Device.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ConstantBuffer.h"
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
	switch (bd->BindFlags)
	{
	case BufferType::BIND_VERTEX_BUFFER:
	{
		*buffer = new VertexBuffer(bd->ByteWidth, bd->StructureByteStride, sd->pSysMem);
	}
	break;
	case BufferType::BIND_INDEX_BUFFER:
	{
		*buffer = new IndexBuffer(bd->ByteWidth, bd->StructureByteStride, sd->pSysMem);
	}
	break;
	case BufferType::BIND_CONSTANT_BUFFER:
	{
		*buffer = new ConstantBuffer(bd->ByteWidth, bd->StructureByteStride, sd->pSysMem);
	}
	break;
	}
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

void Device::CreateSamplerState(SAMPLER_DESC *desc, SamplerState **pSamplerState)
{
	*pSamplerState = new SamplerState(desc);
}
