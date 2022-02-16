#pragma once
#include "CoreAPI.h"
#include "Math/Math.h"

struct InputData
{
	virtual ~InputData() {}
};

struct OutputData
{
	virtual ~OutputData() {}
};

struct CBuffer
{
	virtual ~CBuffer() {}
};

class CORE_API IVertexShader
{
public:
	virtual Vec4f vertex() = 0;
	/*InputData *input;*/
	CBuffer *buffer;
};