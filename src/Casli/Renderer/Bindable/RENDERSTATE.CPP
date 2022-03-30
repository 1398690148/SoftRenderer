#include "RenderState.h"

BRenderState::BRenderState(Graphics & gfx, int cullMode)
{
	if (cullMode == 1)
	{
		pRenderState.m_CullFaceMode = CullFaceMode::CULL_FRONT;
	}
	else if (cullMode == 2)
	{
		pRenderState.m_CullFaceMode = CullFaceMode::CULL_BACK;
	}
	else
	{
		pRenderState.m_CullFaceMode = CullFaceMode::CULL_DISABLE;
	}
}

void BRenderState::Bind(Graphics & gfx)
{
	GetContext(gfx)->SetRenderState(pRenderState);
}
