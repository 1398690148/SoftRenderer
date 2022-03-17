#include "Topology.h"

Topology::Topology(Graphics &gfx, PRIMITIVE_TOPOLOGY type) : type(type)
{}

void Topology::Bind(Graphics &gfx) noexcept
{
	GetContext(gfx)->IASetPrimitiveTopology(type);
}