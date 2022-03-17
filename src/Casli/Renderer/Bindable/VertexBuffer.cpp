#include "VertexBuffer.h"

VertexBuffer::VertexBuffer(Graphics& gfx, const Dvtx::VertexBuffer& vbuf)
	: stride((UINT)vbuf.GetLayout().Size())
{
	BUFFER_DESC bd = {};
	bd.BindFlags = BIND_VERTEX_BUFFER;
	bd.ByteWidth = UINT(vbuf.SizeBytes());
	bd.StructureByteStride = stride;
	SUBRESOURCE_DATA sd = {};
	sd.pSysMem = vbuf.GetData();
	GetDevice(gfx)->CreateBuffer(&bd, &sd, &pVertexBuffer);
}

void VertexBuffer::Bind(Graphics & gfx)
{
	const UINT offset = 0u;
	GetContext(gfx)->IASetVertexBuffers(pVertexBuffer, &stride, &offset);
}
