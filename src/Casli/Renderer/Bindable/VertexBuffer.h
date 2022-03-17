#pragma once
#include "Bindable.h"
#include "Vertex.h"

class VertexBuffer : public Bindable
{
public:
	template<typename V>
	VertexBuffer(Graphics &gfx, std::vector<V> &vertex)
	{
		BUFFER_DESC bd = {};
		bd.BindFlags = BufferType::BIND_VERTEX_BUFFER;
		bd.StructureByteStride = sizeof(V);
		bd.ByteWidth = vertex.size() * sizeof(V);
		SUBRESOURCE_DATA sd = {};
		sd.pSysMem = vertex.data();
		GetDevice(gfx)->CreateBuffer(&bd, &sd, &pVertexBuffer);
	}
	VertexBuffer(Graphics& gfx, const Dvtx::VertexBuffer& vbuf);
	void Bind(Graphics &gfx);
private:
	unsigned int stride;
	SRIBuffer *pVertexBuffer{};
};