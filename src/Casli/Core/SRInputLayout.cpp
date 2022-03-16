#include "SRInputLayout.h"

SRInputLayout::SRInputLayout(const INPUT_ELEMENT_DESC *SRInputLayout, unsigned int s)
	: size(s)
{
	pInputLayout = new INPUT_ELEMENT_DESC[s];
	memcpy(pInputLayout, SRInputLayout, sizeof(INPUT_ELEMENT_DESC) * s);
}

SRInputLayout::~SRInputLayout()
{
	delete pInputLayout;
}

unsigned int SRInputLayout::getSize() const
{
	return size;
}

const INPUT_ELEMENT_DESC *SRInputLayout::getData(unsigned int idx)
{
	return &pInputLayout[idx];
}
