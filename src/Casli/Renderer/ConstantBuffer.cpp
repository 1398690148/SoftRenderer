#include "ConstantBuffer.h"

ConstantBuffer::ConstantBuffer(Graphics &gfx, glm::mat4 &matrix) : matrix(matrix)
{
	BUFFER_DESC constantDesc = {};
	constantDesc.BindFlags = BIND_CONSTANT_BUFFER;
	constantDesc.ByteWidth = sizeof(glm::mat4x4);
	constantDesc.StructureByteStride = sizeof(glm::mat4x4);
	SUBRESOURCE_DATA constantSD;
	constantSD.pSysMem = &this->matrix;
	GetDevice(gfx)->CreateBuffer(&constantDesc, &constantSD, &pConstantBuffer);
}

void ConstantBuffer::Bind(Graphics & gfx)
{
	GetContext(gfx)->VSSetConstantBuffers(pConstantBuffer);
}
