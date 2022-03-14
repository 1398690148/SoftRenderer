#include "PixelConstantBuffer.h"

PixelConstantBuffer::PixelConstantBuffer(Graphics &gfx, unsigned char *buffer, unsigned int slot, unsigned int ByteWidth) : offset(slot)
{
	BUFFER_DESC constantDesc = {};
	constantDesc.BindFlags = BIND_CONSTANT_BUFFER;
	constantDesc.ByteWidth = ByteWidth;
	constantDesc.StructureByteStride = 0;
	SUBRESOURCE_DATA constantSD;
	constantSD.pSysMem = buffer;
	GetDevice(gfx)->CreateBuffer(&constantDesc, &constantSD, &pConstantBuffer);
}

void PixelConstantBuffer::ResetBuffer(unsigned char *buffer)
{
	memcpy(pConstantBuffer->GetBuffer(0), buffer, pConstantBuffer->GetByteWidth());
}

void PixelConstantBuffer::Bind(Graphics &gfx)
{
	GetContext(gfx)->PSSetConstantBuffers(offset, 1, pConstantBuffer);
}
