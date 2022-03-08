#include "Drawable.h"
#include <Graphics.h>

Device *Drawable::GetDevice(Graphics & gfx)
{
	return gfx.pDevice.get();
}

DeviceContext *Drawable::GetContext(Graphics & gfx)
{
	return gfx.pContext.get();
}
