#pragma once
#include <Bindable.h>
#include <Drawable.h>

class BindableFactory
{
public:
	static std::shared_ptr<Bindable> CreateBindable(Graphics &gfx, BindType type, std::vector<std::string> content);
private:
	BindableFactory() = delete;
	~BindableFactory() = delete;
};