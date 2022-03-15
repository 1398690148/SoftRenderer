#pragma once
#include <glm/mat4x4.hpp>
#include <RendererAPI.h>
#include <Core.h>
#include <Blender.h>

class Device;
class DeviceContext;
class Graphics;

//���л��ƶ���Ļ���
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
	static Device *GetDevice(Graphics &gfx);
	static DeviceContext* GetContext(Graphics &gfx);
	Graphics &pGfx;
	glm::mat4 ModelMatrix;
	ShaderState state;
	Blender *pBlender{};
};