#pragma once
#include "Bindable.h"

class Topology : public Bindable
{
public:
	Topology(Graphics& gfx, PRIMITIVE_TOPOLOGY type);
	void Bind(Graphics& gfx) noexcept override;
protected:
	PRIMITIVE_TOPOLOGY type;
};