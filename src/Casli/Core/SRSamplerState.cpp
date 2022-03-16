#include "SRSamplerState.h"

SRSamplerState::SRSamplerState(SAMPLER_DESC *desc)
{
	pSamplerState = *desc;
}
