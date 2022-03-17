#include "PixelShader.h"

PixelShader::PixelShader(Graphics &gfx, SRIPixelShader *shader)
{
	pShader = shader;
}

void PixelShader::Bind(Graphics & gfx) noexcept
{
	GetContext(gfx)->PSSetShader(pShader);
}
