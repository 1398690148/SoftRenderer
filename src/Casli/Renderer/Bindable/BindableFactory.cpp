#include "BindableFactory.h"
#include <Tools.h>
#include <VertexShader.h>
#include <PixelShader.h>
#include <SampleTextureVS.h>
#include <SampleTexturePS.h>
#include <LambertVS.h>
#include <LambertPS.h>
#include <AlphaTestVS.h>
#include <AlphaTestPS.h>
#include <AlphaBlendVS.h>
#include <AlphaBlendPS.h>
#include <PointLightPS.h>
#include <SpotLightPS.h>
#include <BlinnPhongPS.h>
#include <Texture.h>
#include <Sampler.h>
#include <InputLayout.h>
#include <ConstantBuffers.h>
#include <Topology.h>
#include <Blender.h>

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
		else if (content[0] == "AlphaTestVS")
		{
			return std::make_shared<VertexShader>(gfx, new AlphaTestVS());
		}
		else if (content[0] == "AlphaBlendVS")
		{
			return std::make_shared<VertexShader>(gfx, new AlphaBlendVS());
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
		else if (content[0] == "SpotLightPS")
		{
			return std::make_shared<PixelShader>(gfx, new SpotLightPS());
		}
		else if (content[0] == "AlphaTestPS")
		{
			return std::make_shared<PixelShader>(gfx, new AlphaTestPS());
		}
		else if (content[0] == "AlphaBlendPS")
		{
			return std::make_shared<PixelShader>(gfx, new AlphaBlendPS());
		}
		else if (content[0] == "BlinnPhongPS")
		{
			return std::make_shared<PixelShader>(gfx, new BlinnPhongPS());
		}
	}
	break;
	case BTexture2D:
	{
		std::string path = Tools::parseStr(content[0]);
		int mipLevel = Tools::parseInt(content[1]);
		int idx = Tools::parseInt(content[2]);
		return std::make_shared<Texture>(gfx, path.c_str(), mipLevel, idx);
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
		bool blending = Tools::parseBool(content[0]);
		return std::make_shared<Blender>(gfx, blending, std::nullopt);
	}
	break;
	case BCullBack:
	{

	}
	break;
	}
	return std::shared_ptr<Bindable>();
}
