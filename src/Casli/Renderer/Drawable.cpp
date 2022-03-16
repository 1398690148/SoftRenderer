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

void Drawable::SetBlendState(bool flag)
{
	pBlender = new Blender(pGfx, flag, {});
}

SRDevice *Drawable::GetDevice(Graphics & gfx)
{
	return gfx.pDevice.get();
}

SRDeviceContext *Drawable::GetContext(Graphics & gfx)
{
	return gfx.pContext.get();
}
