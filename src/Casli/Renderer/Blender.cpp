#include "Blender.h"
#include "Core.h"

Blender::Blender(Graphics & gfx, bool blending) : blending(blending)
{
	BLEND_DESC desc = { 0 };
	desc.AlphaToCoverageEnable = false;
	desc.RenderTarget[0].BlendEnable = true;
	desc.RenderTarget[0].SrcBlend = BLEND_SRC_ALPHA;
	desc.RenderTarget[0].DestBlend = BLEND_INV_SRC_ALPHA;
	desc.RenderTarget[0].BlendOp = BLEND_OP_ADD;
	desc.RenderTarget[0].SrcBlendAlpha = BLEND_INV_SRC_ALPHA;
	desc.RenderTarget[0].DestBlendAlpha = BLEND_ZERO;
	desc.RenderTarget[0].BlendOpAlpha = BLEND_OP_ADD;
	desc.RenderTarget[0].RenderTargetWriteMask = 0;
	GetDevice(gfx)->CreateBlendState(&desc, &pBlendState);
}

Blender::Blender(const Blender &blender)
{
	pBlendState = blender.pBlendState;
	blending = blender.blending;
}

void Blender::Bind(Graphics & gfx)
{
	GetContext(gfx)->OMSetBlendState(&pBlendState, nullptr, 0);
}

