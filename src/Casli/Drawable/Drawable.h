#pragma once
#include <glm/mat4x4.hpp>
#include <SRCore.h>
#include "Bindable.h"

class SRDevice;
class SRDeviceContext;
class Graphics;
class IndexBuffer;

typedef enum BindType
{
	BVertexShader,
	BPixelShader,
	BTexture2D,
	BSampler,
	BVSConstantBuffer,
	BPSConstantBuffer,
	BTopology,
	BBlendState,
	BRenderStates,
};

//所有绘制对象的基类
class Drawable
{
public:
	Drawable() = default;
	Drawable(const Drawable&) = delete;
	void Draw(Graphics& gfx) const;
	virtual glm::mat4 GetTransformXM() const = 0;
	virtual glm::mat4 GetInitTransform() const = 0;
	void AddBind(std::shared_ptr<Bindable> bind);
protected:
	std::vector<std::shared_ptr<Bindable>> binds;
};