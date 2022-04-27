#include <Texture.h>
#include "stb_image/stb_image.cpp"
#include <tbb/parallel_for.h>
#include <iostream>

Texture::Texture(Graphics& gfx, const char *path, int mipMapLevel, int idx) : index(idx)
{
	stbi_set_flip_vertically_on_load(false);
	unsigned char* data = stbi_load(path, &width, &height, &channels, 0);
	if (data != nullptr)
	{
		int size = width * height * channels;
		TEXTURE2D_DESC textureDesc = {};
		textureDesc.Width = width;
		textureDesc.Height = height;
		textureDesc.MipLevels = mipMapLevel;
		textureDesc.ArraySize = 1;
		textureDesc.Format = FORMAT_UNSIGNEDCHAR;
		textureDesc.BindFlags = BIND_SHADER_RESOURCE;

		SUBRESOURCE_DATA texSd = {};
		texSd.pSysMem = data;
		texSd.SysMemPitch = width * channels;

		if (!GetDevice(gfx)->CreateTexture2D(&textureDesc, &texSd, &pTexture))
		{
			std::cerr << path << " Read Failed" << std::endl;
			exit(-1);
		}
		if (textureDesc.MipLevels > 1)
		{
			GetContext(gfx)->GenerateMips(pTexture);
		}
	}
}

void Texture::Bind(Graphics &gfx)
{
	GetContext(gfx)->PSSetShaderResources(index, &pTexture);
}

