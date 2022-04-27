#pragma once
#include "Bindable.h"

template<typename C>
class ConstantBuffer : public Bindable
{
public:
	void Update(Graphics &gfx, int offset, const C &consts)
	{
		this->offset = offset;
		memcpy(pConstantBuffer->GetBuffer(0), (const void *)&consts, sizeof(C));
	}
	ConstantBuffer(Graphics &gfx, const C &consts)
	{
		BUFFER_DESC cbd;
		cbd.BindFlags = BIND_CONSTANT_BUFFER;
		cbd.ByteWidth = sizeof(consts);
		cbd.StructureByteStride = 0u;

		SUBRESOURCE_DATA csd = {};
		csd.pSysMem = &consts;
		GetDevice(gfx)->CreateBuffer(&cbd, &csd, &pConstantBuffer);
	}

	ConstantBuffer(Graphics& gfx)
	{
		BUFFER_DESC cbd;
		cbd.BindFlags = BIND_CONSTANT_BUFFER;
		cbd.ByteWidth = sizeof(C);
		cbd.StructureByteStride = 0u;


		GetDevice(gfx)->CreateBuffer(&cbd, nullptr, &pConstantBuffer);
	}

protected:
	SRIBuffer *pConstantBuffer{};
	int offset;
};

template<typename C>
class VertexConstantBuffer : public ConstantBuffer<C>
{
	using ConstantBuffer<C>::pConstantBuffer;
	using Bindable::GetContext;
public:
	using ConstantBuffer<C>::ConstantBuffer;
	void Bind(Graphics& gfx)override
	{
		GetContext(gfx)->VSSetConstantBuffers(0u, sizeof(C), pConstantBuffer);
	}
};

template<typename C>
class PixelConstantBuffer : public ConstantBuffer<C>
{
	using ConstantBuffer<C>::pConstantBuffer;
	using Bindable::GetContext;
public:
	using ConstantBuffer<C>::ConstantBuffer;
	void Bind(Graphics& gfx)override
	{
		GetContext(gfx)->PSSetConstantBuffers(offset, 1u, pConstantBuffer);
	}
};