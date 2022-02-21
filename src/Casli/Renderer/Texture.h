#pragma once
#include "Bindable.h"

class Texture : public Bindable
{
public:
	Texture(Graphics& gfx, const char *path, int idx);
	void Bind(Graphics& gfx) override;
private:
	int width;
	int height;
	int channels;
	int index;
	Texture2D *pTexture;
};