#include "Vertex.h"

namespace Dvtx
{
	// VertexLayout
	const VertexLayout::Element& VertexLayout::ResolveByIndex(size_t i) const
	{
		return elements[i];
	}
	VertexLayout& VertexLayout::Append(ElementType type)
	{
		elements.emplace_back(type, Size());
		return *this;
	}
	size_t VertexLayout::Size() const
	{
		return elements.empty() ? 0u : elements.back().GetOffsetAfter();
	}
	size_t VertexLayout::GetElementCount() const
	{
		return elements.size();
	}
	std::vector<INPUT_ELEMENT_DESC> VertexLayout::GetD3DLayout() const
	{
		std::vector<INPUT_ELEMENT_DESC> desc;
		desc.reserve(GetElementCount());
		for (const auto& e : elements)
		{
			desc.push_back(e.GetDesc());
		}
		return desc;
	}


	// VertexLayout::Element
	VertexLayout::Element::Element(ElementType type, size_t offset)
		:
		type(type),
		offset(offset)
	{}
	size_t VertexLayout::Element::GetOffsetAfter() const
	{
		return offset + Size();
	}
	size_t VertexLayout::Element::GetOffset() const
	{
		return offset;
	}
	size_t VertexLayout::Element::Size() const
	{
		return SizeOf(type);
	}
	constexpr size_t VertexLayout::Element::SizeOf(ElementType type)
	{
		switch (type)
		{
		case Position2D:
			return sizeof(Map<Position2D>::SysType);
		case Position3D:
			return sizeof(Map<Position3D>::SysType);
		case Texture2D:
			return sizeof(Map<Texture2D>::SysType);
		case Normal:
			return sizeof(Map<Normal>::SysType);
		case Float3Color:
			return sizeof(Map<Float3Color>::SysType);
		case Float4Color:
			return sizeof(Map<Float4Color>::SysType);
		}
		assert("Invalid element type" && false);
		return 0u;
	}
	VertexLayout::ElementType VertexLayout::Element::GetType() const noexcept
	{
		return type;
	}
	INPUT_ELEMENT_DESC VertexLayout::Element::GetDesc() const
	{
		switch (type)
		{
		case Position2D:
			return GenerateDesc<Position2D>(GetOffset());
		case Position3D:
			return GenerateDesc<Position3D>(GetOffset());
		case Texture2D:
			return GenerateDesc<Texture2D>(GetOffset());
		case Normal:
			return GenerateDesc<Normal>(GetOffset());
		case Float3Color:
			return GenerateDesc<Float3Color>(GetOffset());
		case Float4Color:
			return GenerateDesc<Float4Color>(GetOffset());
		}
		assert("Invalid element type" && false);
		return { "INVALID",0,0,0 };
	}


	// Vertex
	Vertex::Vertex(char* pData, const VertexLayout& layout)
		:
	pData(pData),
		layout(layout)
	{
		assert(pData != nullptr);
	}
	ConstVertex::ConstVertex(const Vertex& v)
		:
	vertex(v)
	{}


	// VertexBuffer
	VertexBuffer::VertexBuffer(VertexLayout layout)
		:
	layout(std::move(layout))
	{}
	const char* VertexBuffer::GetData() const
	{
		return buffer.data();
	}
	const VertexLayout& VertexBuffer::GetLayout() const
	{
		return layout;
	}
	size_t VertexBuffer::Size() const
	{
		return buffer.size() / layout.Size();
	}
	size_t VertexBuffer::SizeBytes() const
	{
		return buffer.size();
	}
	Vertex VertexBuffer::Back()
	{
		assert(buffer.size() != 0u);
		return Vertex{ buffer.data() + buffer.size() - layout.Size(),layout };
	}
	Vertex VertexBuffer::Front()
	{
		assert(buffer.size() != 0u);
		return Vertex{ buffer.data(),layout };
	}
	Vertex VertexBuffer::operator[](size_t i)
	{
		assert(i < Size());
		return Vertex{ buffer.data() + layout.Size() * i,layout };
	}
	ConstVertex VertexBuffer::Back() const
	{
		return const_cast<VertexBuffer*>(this)->Back();
	}
	ConstVertex VertexBuffer::Front() const
	{
		return const_cast<VertexBuffer*>(this)->Front();
	}
	ConstVertex VertexBuffer::operator[](size_t i) const
	{
		return const_cast<VertexBuffer&>(*this)[i];
	}
}