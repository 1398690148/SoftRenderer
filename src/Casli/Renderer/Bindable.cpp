#include "Bindable.h"

Device * Bindable::GetDevice(Graphics & gfx)
{
	return gfx.pDevice.get();
}

DeviceContext * Bindable::GetContext(Graphics & gfx)
{
	return gfx.pContext.get();
}
