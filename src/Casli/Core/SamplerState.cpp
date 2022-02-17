#include "SamplerState.h"

SamplerState::SamplerState(SAMPLER_DESC * desc)
{
	pSamplerState = *desc;
}
