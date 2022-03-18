#pragma once
#include <glm/mat4x4.hpp>
#include <SRCore.h>
#include <Blender.h>
#include "Bindable.h"

class SRDevice;
class SRDeviceContext;
class Graphics;
class IndexBuffer;

typedef enum BindType
{
	BVertexShader,
	BPixelShader,
	BSampler,
	BVSConstantBuffer,
	BPSConstantBuffer,
	BTopology,
	BBlendState,
	BCullBack,
};

//���л��ƶ���Ļ���
class Drawable
{
public:
	Drawable() = default;
	Drawable(const Drawable&) = delete;
	void Draw(Graphics& gfx) const;
	void SetCullFaceMode(CullFaceMode cullFaceMode);
	void SetBlendState(bool flag);
	virtual glm::mat4 GetTransformXM() const = 0;
	void AddBind(std::shared_ptr<Bindable> bind);
protected:
	const IndexBuffer* pIndexBuffer = nullptr;
	std::vector<std::shared_ptr<Bindable>> binds;
};