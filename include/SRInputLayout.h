#pragma once
#include "SRCore.h"
#include "SRCoreAPI.h"
#include "SRIBuffer.h"

class CORE_API SRInputLayout
{
public:
	SRInputLayout(const INPUT_ELEMENT_DESC *InputLayout, unsigned int s);
	~SRInputLayout();
	unsigned int getSize() const;
	const INPUT_ELEMENT_DESC *getData(unsigned int offset);
private:
	INPUT_ELEMENT_DESC *pInputLayout{};
	unsigned int size;
};