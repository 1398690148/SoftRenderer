#pragma once
#include "Bindable.h"

class SRIPixelShader;

class PixelShader : public Bindable
{
public:
	PixelShader(Graphics& gfx, SRIPixelShader *shader);
	void Bind(Graphics& gfx) noexcept override;
private:
	SRIPixelShader* pShader;
};