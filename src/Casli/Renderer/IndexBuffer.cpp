#include "IndexBuffer.h"

IndexBuffer::IndexBuffer(Graphics &gfx, std::vector<unsigned int> &indices)
{
	BUFFER_DESC ibd = {};
	ibd.BindFlags = BIND_INDEX_BUFFER;
	ibd.ByteWidth = sizeof(unsigned int) * indices.size();
	ibd.StructureByteStride = sizeof(unsigned int);
	SUBRESOURCE_DATA isd = {};
	isd.pSysMem = &indices[0];
	GetDevice(gfx)->CreateBuffer(&ibd, &isd, &pIndexBuffer);
}

void IndexBuffer::Bind(Graphics & gfx)
{
	GetContext(gfx)->IASetIndexBuffer(pIndexBuffer, 0);

}
