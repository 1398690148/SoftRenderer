#include "VertexConstantBuffer.h"

VertexConstantBuffer::VertexConstantBuffer(Graphics &gfx, std::vector<glm::mat4> &matrix) : matrix(matrix)
{
	BUFFER_DESC constantDesc = {};
	constantDesc.BindFlags = BIND_CONSTANT_BUFFER;
	constantDesc.ByteWidth = sizeof(glm::mat4) * matrix.size();
	constantDesc.StructureByteStride = sizeof(glm::mat4);
	SUBRESOURCE_DATA constantSD;
	constantSD.pSysMem = &this->matrix[0];
	GetDevice(gfx)->CreateBuffer(&constantDesc, &constantSD, &pConstantBuffer);
}

VertexConstantBuffer::VertexConstantBuffer(Graphics & gfx, unsigned int offset, unsigned char *buffer, int byteWidth)
{
	BUFFER_DESC constantDesc = {};
	constantDesc.BindFlags = BIND_CONSTANT_BUFFER;
	constantDesc.ByteWidth = byteWidth;
	constantDesc.StructureByteStride = 0;
	SUBRESOURCE_DATA constantSD;
	constantSD.pSysMem = buffer;
	GetDevice(gfx)->CreateBuffer(&constantDesc, &constantSD, &pConstantBuffer);
}

void VertexConstantBuffer::Bind(Graphics &gfx)
{
	GetContext(gfx)->VSSetConstantBuffers(0, 1, pConstantBuffer);
}

void VertexConstantBuffer::SetMatrix(std::vector<glm::mat4> matrix)
{
	memcpy(pConstantBuffer->GetBuffer(0), &matrix[0], sizeof(glm::mat4) * matrix.size());
}
