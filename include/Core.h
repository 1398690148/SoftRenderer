#pragma once
#include <string>

#define PI std::acos(-1)

enum BufferType
{
	BIND_VERTEX_BUFFER,
	BIND_INDEX_BUFFER,
	BIND_CONSTANT_BUFFER,
	BIND_RENDER_TARGET,
	BIND_DEPTH_STENCIL,
	BIND_SHADER_RESOURCE,
};

enum DataFormat
{
	FORMAT_UNSIGNEDCHAR,
	FORMAT_INT,
	FORMAT_FLOAT,
};

struct BUFFER_DESC
{
	BufferType BindFlags;
	unsigned int ByteWidth;
	unsigned int StructureByteStride;
};

struct TEXTURE2D_DESC
{
	unsigned int Width;
	unsigned int Height;
	unsigned int MipLevels;
	unsigned int ArraySize;
	DataFormat Format;
	BufferType BindFlags;
};

struct VIEWPORT
{
	int Width;
	int Height;
	int TopLeftX;
	int TopLeftY;
	int MinDepth;
	int MaxDepth;
};

struct INPUT_ELEMENT_DESC
{
	std::string SemanticName;
	unsigned int typeSize;
	unsigned int Size;
	unsigned int Offset;
};

struct SUBRESOURCE_DATA
{
	const void *pSysMem;
	//从纹理的一行开始到下一行的距离
	unsigned int SysMemPitch;
};

enum PRIMITIVE_TOPOLOGY
{
	PRIMITIVE_TOPOLOGY_LINELIST,
	PRIMITIVE_TOPOLOGY_TRIANGLELIST,
};

struct DEPTH_STENCIL_VIEW_DESC
{
	DataFormat Format;
	unsigned int width;
	unsigned int height;
};

enum FILTER
{
	FILTER_POINT_MIP_POINT,
	FILTER_POINT_MIP_LINEAR,
	FILTER_LINEAR_MIP_POINT,
	FILTER_LINEAR_MIP_LINEAR,
};

enum TEXTURE_ADDRESS_MODE
{
	TEXTURE_ADDRESS_WRAP,
	TEXTURE_ADDRESS_MIRROR,
	TEXTURE_ADDRESS_CLAMP,
	TEXTURE_ADDRESS_BORDER,
};

struct SAMPLER_DESC
{
	FILTER Filter;
	TEXTURE_ADDRESS_MODE AddressU;
	TEXTURE_ADDRESS_MODE AddressV;
	TEXTURE_ADDRESS_MODE AddressW;
	unsigned char borderColor[4];
};

enum BLEND {
	BLEND_ZERO = 1,
	BLEND_ONE = 2,
	BLEND_SRC_COLOR = 3,
	BLEND_INV_SRC_COLOR = 4,
	BLEND_SRC_ALPHA = 5,
	BLEND_INV_SRC_ALPHA = 6,
	BLEND_DEST_ALPHA = 7,
	BLEND_INV_DEST_ALPHA = 8,
	BLEND_DEST_COLOR = 9,
	BLEND_INV_DEST_COLOR = 10,
	BLEND_SRC_ALPHA_SAT = 11,
	BLEND_BLEND_FACTOR = 14,
	BLEND_INV_BLEND_FACTOR = 15,
	BLEND_SRC1_COLOR = 16,
	BLEND_INV_SRC1_COLOR = 17,
	BLEND_SRC1_ALPHA = 18,
	BLEND_INV_SRC1_ALPHA = 19
};

enum BLEND_OP {
	BLEND_OP_ADD = 1,
	BLEND_OP_SUBTRACT = 2,
	BLEND_OP_REV_SUBTRACT = 3,
	BLEND_OP_MIN = 4,
	BLEND_OP_MAX = 5
};

struct RENDER_TARGET_BLEND_DESC {
	bool BlendEnable;
	BLEND SrcBlend;
	BLEND DestBlend;
	BLEND_OP BlendOp;
	BLEND SrcBlendAlpha;
	BLEND DestBlendAlpha;
	BLEND_OP BlendOpAlpha;
	unsigned char RenderTargetWriteMask;
};

struct BLEND_DESC
{
	bool AlphaToCoverageEnable;
	bool IndependentBlendEnable;
	RENDER_TARGET_BLEND_DESC RenderTarget[8];
};