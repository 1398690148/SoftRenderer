#pragma once
#include "CoreAPI.h"
#include "IBuffer.h"

class CORE_API InputLayout
{
public:
	InputLayout(const INPUT_ELEMENT_DESC *InputLayout, unsigned int s);
	~InputLayout();
	unsigned int getSize() const;
	const INPUT_ELEMENT_DESC *getData(unsigned int offset);
private:
	INPUT_ELEMENT_DESC *pInputLayout;
	unsigned int size;
};