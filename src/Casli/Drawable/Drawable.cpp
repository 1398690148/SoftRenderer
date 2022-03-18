#include "Drawable.h"
#include <Graphics.h>
#include <IndexBuffer.h>

void Drawable::Draw(Graphics& gfx) const
{
	for (auto& b : binds)
	{
		b->Bind(gfx);
	}
	gfx.DrawIndexed();
}

void Drawable::SetCullFaceMode(CullFaceMode cullFaceMode)
{
	//state.m_CullFaceMode = cullFaceMode;
}

void Drawable::SetBlendState(bool flag)
{
	//pBlender = new Blender(pGfx, flag, {});
}

void Drawable::AddBind(std::shared_ptr<Bindable> bind)
{
	// special case for index buffer
	if (typeid(*bind) == typeid(IndexBuffer))
	{
		assert("Binding multiple index buffers not allowed" && pIndexBuffer == nullptr);
		pIndexBuffer = &static_cast<IndexBuffer&>(*bind);
	}
	binds.push_back(std::move(bind));
}
