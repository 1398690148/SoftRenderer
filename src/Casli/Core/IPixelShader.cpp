#include "IPixelShader.h"

IPixelShader::IPixelShader()
{
	textures = new Texture2D*[16];
	samplers = new SamplerState*[4];
}

IPixelShader::~IPixelShader()
{ 
	delete buffer; 
}
