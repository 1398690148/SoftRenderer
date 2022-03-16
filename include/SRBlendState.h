#pragma once
#include "SRCoreAPI.h"
#include "SRCore.h"

class CORE_API SRBlendState
{
	friend class SRDeviceContext;
public:
	SRBlendState(BLEND_DESC *desc);
private:
	BLEND_DESC blendDesc;
};