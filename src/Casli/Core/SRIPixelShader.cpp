#include "SRIPixelShader.h"

SRIPixelShader::SRIPixelShader()
{
	textures = new SRTexture2D*[16];
	for (int i = 0; i < 16; i++)
	{
		textures[i] = nullptr;
	}
	samplers = new SRSamplerState*[4];
	for (int i = 0; i < 4; i++)
	{
		samplers[i] = nullptr;
	}
}

SRIPixelShader::~SRIPixelShader()
{ 
	delete cbuffer; 
}
