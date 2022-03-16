#pragma once
#include "SRCore.h"
#include "SRCoreAPI.h"
#include "SRIBuffer.h"

class CORE_API SRSamplerState
{
	friend class SRTexture2D;
public:
	SRSamplerState(SAMPLER_DESC *desc);
private:
	SAMPLER_DESC pSamplerState;
};