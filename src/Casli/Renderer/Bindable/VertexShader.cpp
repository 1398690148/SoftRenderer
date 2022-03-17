#include "VertexShader.h"

VertexShader::VertexShader(Graphics &gfx, SRIVertexShader *shader)
{
	pShader = shader;
}

void VertexShader::Bind(Graphics & gfx) noexcept
{
	GetContext(gfx)->VSSetShader(pShader);
}
