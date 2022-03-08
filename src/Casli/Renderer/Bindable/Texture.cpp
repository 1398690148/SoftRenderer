#include <algorithm>
#include <Texture.h>
#include "stb_image/stb_image.cpp"

Texture::Texture(Graphics& gfx, const char *path, int mipMapLevel, int idx) : index(idx)
{
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(path, &width, &height, &channels, 0);
	if (data != nullptr)
	{
		int size = width * height * channels;
		unsigned char *buffer = new unsigned char[size];
		for (int i = 0; i < size; i++)
		{
			buffer[i] = data[i];
			if ((i + 1) % 4 == 0 && buffer[i] != 0)
			{
				int k = buffer[i];
			}
		}
		TEXTURE2D_DESC textureDesc = {};
		textureDesc.Width = width;
		textureDesc.Height = height;
		textureDesc.MipLevels = mipMapLevel;
		textureDesc.ArraySize = 1;
		textureDesc.Format = FORMAT_UNSIGNEDCHAR;
		textureDesc.BindFlags = BIND_SHADER_RESOURCE;

		SUBRESOURCE_DATA texSd = {};
		texSd.pSysMem = buffer;
		texSd.SysMemPitch = width * channels;

		if (!GetDevice(gfx)->CreateTexture2D(&textureDesc, &texSd, &pTexture))
		{
			throw;
		}
		if (textureDesc.MipLevels > 1)
		{
			GetContext(gfx)->GenerateMips(pTexture);
		}
	}
	stbi_image_free(data);
	
}

void Texture::Bind(Graphics &gfx)
{
	GetContext(gfx)->PSSetShaderResources(index, &pTexture);
}

