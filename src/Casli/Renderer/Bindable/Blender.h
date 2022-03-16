#pragma once
#include "Bindable.h"
#include <optional>
#include <array>

class BlendState;

class RENDERER_API Blender : public Bindable
{
public:
	Blender(Graphics &gfx, bool blending, std::optional<float> factors_in);
	void Bind(Graphics &gfx) override;
	//static Blender Resolve(Graphics &gfx, bool blending);
private:
	SRBlendState *pBlendState{};
	bool blending;
	std::optional<std::array<float, 4>> factors;
};