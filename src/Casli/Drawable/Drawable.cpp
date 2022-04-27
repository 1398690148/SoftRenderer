#include "Drawable.h"
#include <IndexBuffer.h>

void Drawable::Draw(Graphics& gfx) const
{
	for (auto& b : binds)
	{
		b->Bind(gfx);
	}
	gfx.DrawIndexed();
}

void Drawable::AddBind(std::shared_ptr<Bindable> bind)
{
	binds.push_back(std::move(bind));
}
