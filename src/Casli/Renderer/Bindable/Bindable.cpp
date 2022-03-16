#include "Bindable.h"

SRDevice *Bindable::GetDevice(Graphics & gfx)
{
	return gfx.pDevice.get();
}

SRDeviceContext *Bindable::GetContext(Graphics & gfx)
{
	return gfx.pContext.get();
}
