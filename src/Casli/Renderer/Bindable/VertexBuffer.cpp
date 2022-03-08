#include "VertexBuffer.h"

VertexBuffer::VertexBuffer(Graphics & gfx, std::vector<Vertex> &vertex)
{
	BUFFER_DESC bd = {};
	bd.BindFlags = BufferType::BIND_VERTEX_BUFFER;
	bd.StructureByteStride = sizeof(Vertex);
	bd.ByteWidth = vertex.size() * sizeof(Vertex);
	SUBRESOURCE_DATA sd = {};
	sd.pSysMem = &vertex[0];
	GetDevice(gfx)->CreateBuffer(&bd, &sd, &pVertexBuffer);
}

void VertexBuffer::Bind(Graphics & gfx)
{
	const UINT stride = sizeof(Vertex);
	const UINT offset = 0u;
	float p = *(float *)pVertexBuffer->GetBuffer(0);
	GetContext(gfx)->IASetVertexBuffers(pVertexBuffer, &stride, &offset);
}
