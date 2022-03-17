#pragma once
#include <Graphics.h>

class Bindable
{
public:
	virtual void Bind(Graphics& gfx) = 0;
	virtual ~Bindable() = default;
protected:
	static SRDevice *GetDevice(Graphics &gfx);
	static SRDeviceContext* GetContext(Graphics &gfx);
};