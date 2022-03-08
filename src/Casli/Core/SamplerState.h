#pragma once
#include "CoreAPI.h"
#include "IBuffer.h"

class CORE_API SamplerState
{
	friend class Texture2D;
public:
	SamplerState(SAMPLER_DESC *desc);
private:
	SAMPLER_DESC pSamplerState;
};