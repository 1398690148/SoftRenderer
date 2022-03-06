#pragma once

class Device;
class DeviceContext;
class Graphics;
//���л��ƶ���Ļ���
class Drawable
{
public:
	Drawable() {}
	virtual void Bind(Graphics &gfx, unsigned char *ConstantBuffer) = 0;
	virtual void Draw(Graphics &gfx) = 0;
protected:
	static Device *GetDevice(Graphics &gfx);
	static DeviceContext* GetContext(Graphics &gfx);
};