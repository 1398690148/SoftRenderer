#pragma once
#include "Bindable.h"

class SRIVertexShader;

class VertexShader : public Bindable
{
public:
	VertexShader(Graphics& gfx, SRIVertexShader *shader);
	void Bind(Graphics& gfx) noexcept override;
private:
	SRIVertexShader* pShader;
};