#pragma once
#include "CoreAPI.h"
#include "Core.h"

class CORE_API SamplerState
{
public:
	SamplerState(SAMPLER_DESC *desc);
private:
	SAMPLER_DESC pSamplerState;
};