#pragma once

class Device;
class DeviceContext;
class Graphics;

//所有绘制对象的基类
class Drawable
{
public:
	Drawable() {}
	virtual void Draw(Graphics &gfx) = 0;
	virtual void Bind(Graphics &gfx, unsigned char *ConstantBuffer, size_t size) = 0;

protected:
	static Device *GetDevice(Graphics &gfx);
	static DeviceContext* GetContext(Graphics &gfx);
};