#include "InputLayout.h"

InputLayout::InputLayout(const INPUT_ELEMENT_DESC *InputLayout, unsigned int s)
	: size(s)
{
	pInputLayout = new INPUT_ELEMENT_DESC[s];
	memcpy(pInputLayout, InputLayout, sizeof(INPUT_ELEMENT_DESC) * s);
}

InputLayout::~InputLayout()
{
	delete pInputLayout;
}

unsigned int InputLayout::getSize() const
{
	return size;
}

const INPUT_ELEMENT_DESC *InputLayout::getData(unsigned int idx)
{
	return &pInputLayout[idx];
}
