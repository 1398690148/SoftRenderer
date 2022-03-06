#pragma once
#include "Bindable.h"

class BlendState;

class Blender : public Bindable
{
public:
	Blender(Graphics &gfx, bool blending);
	Blender(const Blender &blender);
	void Bind(Graphics &gfx) override;
	//static Blender Resolve(Graphics &gfx, bool blending);
private:
	BlendState *pBlendState{};
	bool blending;
};