#pragma once
#include <Bindable.h>

class InputLayout : public Bindable
{
public:
	InputLayout(Graphics& gfx, const std::vector<INPUT_ELEMENT_DESC> &layout);
	~InputLayout();
	void Bind(Graphics& gfx)override;
protected:
	SRInputLayout *pInputLayout{};
};