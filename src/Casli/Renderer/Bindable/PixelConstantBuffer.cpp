#include "PixelConstantBuffer.h"

PixelConstantBuffer::PixelConstantBuffer(Graphics & gfx, unsigned int offset, unsigned char *buffer, int byteWidth) : StartOffset(offset)
{
	BUFFER_DESC constantDesc = {};
	constantDesc.BindFlags = BIND_CONSTANT_BUFFER;
	constantDesc.ByteWidth = byteWidth;
	constantDesc.StructureByteStride = 0;
	SUBRESOURCE_DATA constantSD;
	constantSD.pSysMem = buffer;
	GetDevice(gfx)->CreateBuffer(&constantDesc, &constantSD, &pConstantBuffer);
}

void PixelConstantBuffer::Bind(Graphics &gfx)
{
	GetContext(gfx)->PSSetConstantBuffers(StartOffset, 1, pConstantBuffer);
}

void PixelConstantBuffer::SetMatrix(glm::mat4 matrix)
{
	memcpy(pConstantBuffer->GetBuffer(0), &matrix, sizeof(glm::mat4));
}
