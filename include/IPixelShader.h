#pragma once
#include "CoreAPI.h"
#include "Math/Math.h"

typedef struct InputData
{
	virtual ~InputData() {}
} InputData;

typedef struct CBuffer
{
	virtual ~CBuffer() {}
} CBuffer;

class CORE_API IPixelShader
{
public:
	virtual bool fragment(InputData *in, Vec4f &color) = 0;
	CBuffer *buffer;
};