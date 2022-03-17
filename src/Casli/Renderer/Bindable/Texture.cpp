#include <algorithm>
#include <Texture.h>
#include "stb_image/stb_image.cpp"
#include <tbb/blocked_range.h>
#include <tbb/parallel_for.h>

Texture::Texture(Graphics& gfx, const char *path, int mipMapLevel, int idx) : index(idx)
{
	stbi_set_flip_vertically_on_load(false);
	unsigned char* data = stbi_load(path, &width, &height, &channels, 0);
	if (data != nullptr)
	{
		int size = width * height * channels;
		unsigned char *buffer = new unsigned char[size];
		tbb::parallel_for(tbb::blocked_range<size_t>(0, size), [&](const tbb::blocked_range<size_t> &r)
		{
			for (int i = r.begin(); i != r.end(); i++)
			{
				buffer[i] = data[i];
			}
		});
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
	//else
	//{
	//	throw std::string("texture path null");
	//}
	stbi_image_free(data);
	
}

void Texture::Bind(Graphics &gfx)
{
	GetContext(gfx)->PSSetShaderResources(index, &pTexture);
}

