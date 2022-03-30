#pragma once
#include "Bindable.h"

class BRenderState : public Bindable
{
public:
	BRenderState(Graphics& gfx, CullFaceMode mode);
	void Bind(Graphics& gfx) override;
private:
	RenderState pRenderState;
};