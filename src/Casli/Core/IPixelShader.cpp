#include "IPixelShader.h"

IPixelShader::IPixelShader()
{
	textures = new Texture2D*[16];
	for (int i = 0; i < 16; i++)
	{
		textures[i] = nullptr;
	}
	samplers = new SamplerState*[4];
	for (int i = 0; i < 4; i++)
	{
		samplers[i] = nullptr;
	}
}

IPixelShader::~IPixelShader()
{ 
	delete cbuffer; 
}
