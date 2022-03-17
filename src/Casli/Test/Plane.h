#pragma once
#include <VertexBuffer.h>
#include <IndexBuffer.h>
#include <Drawable.h>
#include <ConstantBuffers.h>

class Graphics;
class Blender;

class Plane : public Drawable
{
public:
	Plane(Graphics &gfx, std::string texturePath, int mipLevel, glm::vec3 trans, glm::vec4 rotate, FILTER filter = FILTER_POINT_MIP_POINT);
	glm::mat4 GetTransformXM() const;
private:
	glm::vec3 m_Translate;
	glm::vec4 m_Rotate;
};