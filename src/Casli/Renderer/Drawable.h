#pragma once
#include <glm/mat4x4.hpp>
#include <RendererAPI.h>
#include <SRCore.h>
#include <Blender.h>

class SRDevice;
class SRDeviceContext;
class Graphics;

//所有绘制对象的基类
class RENDERER_API Drawable
{
public:
	Drawable(Graphics &p) : pGfx(p) {}
	virtual void Draw() = 0;
	virtual void Bind(unsigned char *ConstantBuffer, size_t size) = 0;
	void SetModelMatrix(glm::mat4 &matrix);
	glm::mat4 GetModelMatrix();
	void SetCullFaceMode(CullFaceMode cullFaceMode);
	void SetBlendState(bool flag);
protected:
	static SRDevice *GetDevice(Graphics &gfx);
	static SRDeviceContext* GetContext(Graphics &gfx);
	Graphics &pGfx;
	glm::mat4 ModelMatrix;
	ShaderState state;
	Blender *pBlender{};
};