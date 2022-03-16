#include "SRIVertexShader.h"

SRIVertexShader::SRIVertexShader()
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

SRIVertexShader::~SRIVertexShader()
{
	delete cbuffer;
}