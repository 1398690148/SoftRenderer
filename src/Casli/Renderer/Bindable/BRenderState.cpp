#include <BRenderState.h>

BRenderState::BRenderState(Graphics& gfx, CullFaceMode mode, bool mipmapping)
{
	if (mode == 1)
	{
		pRenderState.m_CullFaceMode = CullFaceMode::CULL_FRONT;
	}
	else if (mode == 2)
	{
		pRenderState.m_CullFaceMode = CullFaceMode::CULL_BACK;
	}
	else
	{
		pRenderState.m_CullFaceMode = CullFaceMode::CULL_DISABLE;
	}
	pRenderState.m_Mipmapping = mipmapping;
}

void BRenderState::Bind(Graphics & gfx)
{
	GetContext(gfx)->SetRenderState(pRenderState);
}