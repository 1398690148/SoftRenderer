#pragma once
#include "CoreAPI.h"
#include "Core.h"

class CORE_API BlendState
{
	friend class DeviceContext;
public:
	BlendState(BLEND_DESC *desc);
private:
	BLEND_DESC blendDesc;
};