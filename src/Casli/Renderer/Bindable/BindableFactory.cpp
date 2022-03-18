#include "BindableFactory.h"
#include <Tools.h>
#include <VertexShader.h>
#include <SampleTextureVS.h>
#include <LambertVS.h>
#include <PixelShader.h>
#include <SampleTexturePS.h>
#include <PointLightPS.h>
#include <LambertPS.h>
#include <Sampler.h>
#include <InputLayout.h>
#include <ConstantBuffers.h>
#include <Topology.h>
#include <glm/mat4x4.hpp>
#include <glm/gtx/transform.hpp>

std::shared_ptr<Bindable> BindableFactory::CreateBindable(Graphics &gfx, BindType type, std::vector<std::string> content)
{
	switch (type)
	{
	case BVertexShader:
	{
		content[0] = Tools::parseStr(content[0]);
		if (content[0] == "SampleTextureVS")
		{
			return std::make_shared<VertexShader>(gfx, new SampleTextureVS());
		}
		else if (content[0] == "LambertVS")
		{
			return std::make_shared<VertexShader>(gfx, new LambertVS());
		}
	}
	break;
	case BPixelShader:
	{
		content[0] = Tools::parseStr(content[0]);
		if (content[0] == "SampleTexturePS")
		{
			return std::make_shared<PixelShader>(gfx, new SampleTexturePS());
		}
		else if (content[0] == "LambertPS")
		{
			return std::make_shared<PixelShader>(gfx, new LambertPS());
		}
		else if (content[0] == "PointLightPS")
		{
			return std::make_shared<PixelShader>(gfx, new PointLightPS());
		}
	}
	break;
	case BSampler:
	{
		glm::ivec2 param = Tools::parseVec2(content[0]);
		return std::make_shared<Sampler>(gfx, param.x, (FILTER)param.y);
	}
	break;
	case BVSConstantBuffer:
	{
		glm::mat4 Model = glm::mat4(1.0);
		Model = glm::scale(Model, Tools::parseVec3(content[2]));
		glm::vec4 rotate = Tools::parseVec4(content[1]);
		Model = glm::rotate(Model, glm::radians(rotate.x), glm::vec3(rotate.y, rotate.z, rotate.w));
		Model = glm::translate(Model, Tools::parseVec3(content[0]));
		return std::make_shared<VertexConstantBuffer<glm::mat4>>(gfx, Model);
	}
	break;
	case BPSConstantBuffer:
	{

	}
	break;
	case BTopology:
	{
		return std::make_shared<Topology>(gfx, (PRIMITIVE_TOPOLOGY)Tools::parseInt(content[0]));
	}
	break;
	case BBlendState:
	{

	}
	break;
	case BCullBack:
	{

	}
	break;
	}
	return std::shared_ptr<Bindable>();
}
