#include "Drawable.h"
#include <Graphics.h>

void Drawable::SetModelMatrix(glm::mat4 &matrix)
{
	ModelMatrix = matrix;
}

glm::mat4 Drawable::GetModelMatrix()
{
	return ModelMatrix;
}

void Drawable::SetCullFaceMode(CullFaceMode cullFaceMode)
{
	state.m_CullFaceMode = cullFaceMode;
}

Device *Drawable::GetDevice(Graphics & gfx)
{
	return gfx.pDevice.get();
}

DeviceContext *Drawable::GetContext(Graphics & gfx)
{
	return gfx.pContext.get();
}
