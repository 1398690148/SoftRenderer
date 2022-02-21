#pragma once
#include "../Scene/Graphics.h"

class Bindable
{
public:
	virtual void Bind(Graphics& gfx) = 0;
	virtual ~Bindable() = default;
protected:
	static Device *GetDevice(Graphics &gfx);
	static DeviceContext* GetContext(Graphics &gfx);
};