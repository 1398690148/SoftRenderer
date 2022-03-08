#include "Blender.h"

Blender::Blender(Graphics & gfx, bool blending, std::optional<float> factors_in) : blending(blending)
{
	if (factors_in)
	{
		factors.emplace();
		factors->fill(*factors_in);
	}

	BLEND_DESC desc = { 0 };
	desc.AlphaToCoverageEnable = false;
	desc.RenderTarget[0].BlendEnable = true;
	auto &brt = desc.RenderTarget[0];
	if (blending)
	{
		if (factors_in)
		{
			brt.SrcBlend = BLEND_BLEND_FACTOR;
			brt.DestBlend = BLEND_INV_BLEND_FACTOR;
		}
		else
		{
			brt.SrcBlend = BLEND_SRC_ALPHA;
			brt.DestBlend = BLEND_INV_SRC_ALPHA;
			brt.BlendOp = BLEND_OP_ADD;
		}
	}
	//brt.SrcBlendAlpha = BLEND_INV_SRC_ALPHA;
	//brt.DestBlendAlpha = BLEND_ZERO;
	//brt.BlendOpAlpha = BLEND_OP_ADD;
	//brt.RenderTargetWriteMask = 0;
	GetDevice(gfx)->CreateBlendState(&desc, &pBlendState);
}

void Blender::Bind(Graphics & gfx)
{
	const float* data = factors ? factors->data() : nullptr;
	GetContext(gfx)->OMSetBlendState(&pBlendState, data, 0);
}

