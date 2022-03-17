#include "InputLayout.h"

InputLayout::InputLayout(Graphics &gfx, const std::vector<INPUT_ELEMENT_DESC> &layout)
{
	GetDevice(gfx)->CreateInputLayout(layout.data(), layout.size(), &pInputLayout);
}

InputLayout::~InputLayout()
{
}

void InputLayout::Bind(Graphics &gfx)
{
	GetContext(gfx)->IASetInputLayout(pInputLayout);
}
