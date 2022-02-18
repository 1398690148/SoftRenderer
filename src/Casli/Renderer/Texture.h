#pragma once

class Texture
{
public:
	Texture();
	Texture(const char *path);
	~Texture();
	unsigned char *GetBufferPtr();
	int GetWidth() const;
	int GetHeight() const;
	int GetChannel() const;

private:
	int width;
	int height;
	int channels;

	unsigned char *buffer{};
};