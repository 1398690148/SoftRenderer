#pragma once
#include "Bindable.h"

class BRenderState : public Bindable
{
public:
	BRenderState(Graphics& gfx, int cullMode);
	void Bind(Graphics& gfx) override;
private:
	ShaderState pRenderState;
};