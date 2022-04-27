#include "SceneParse.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <Model.h>
#include <Plane.h>
#include <DirectionalLight.h>
#include <PointLight.h>
#include <SpotLight.h>
#include <BindableFactory.h>
#include <Tools.h>
#include <glm/gtx/transform.hpp>

void SceneParse::parse(const std::string &path, Graphics &gfx)
{
	std::ifstream sceneFile;
	sceneFile.open(path, std::ios::in);

	if (!sceneFile.is_open())
	{
		std::cerr << "File does not exist: " << path << std::endl;
		exit(1);
	}

	std::string line;
	while (std::getline(sceneFile, line))
	{
		if (line.empty())
			continue;

		std::stringstream ss;
		ss << line;
		std::string header;
		ss >> header;
		if (header == "Camera:")
		{
			std::cout << "Camera:=========================================\n";
			{
				std::getline(sceneFile, line);
				m_scene.m_CameraPos = Tools::parseVec3(line);
			}
			{
				std::getline(sceneFile, line);
				m_scene.m_CameraFront = Tools::parseVec3(line);
			}
			{
				std::getline(sceneFile, line);
				m_scene.m_CameraUp = Tools::parseVec3(line);
			}
		}
		else if (header == "Frustum:")
		{
			std::cout << "Frustum:========================================\n";
			{
				std::getline(sceneFile, line);
				m_scene.m_FrustumFovy = Tools::parseFloat(line);
			}
			{
				std::getline(sceneFile, line);
				m_scene.m_FrustumNear = Tools::parseFloat(line);
			}
			{
				std::getline(sceneFile, line);
				m_scene.m_FrustumFar = Tools::parseFloat(line);
			}
		}
		else if (header == "PointLight:")
		{
			std::cout << "PointLight:=====================================\n";
			int offset;
			{
				std::getline(sceneFile, line);
				offset = Tools::parseInt(line);
			}
			glm::vec3 pos;
			{
				std::getline(sceneFile, line);
				pos = Tools::parseVec3(line);
			}
			glm::vec3 color;
			{
				std::getline(sceneFile, line);
				color = Tools::parseVec3(line);
			}
			glm::vec3 atten;
			{
				std::getline(sceneFile, line);
				atten = Tools::parseVec3(line);
			}
			std::shared_ptr<PointLight> light = std::make_shared<PointLight>(gfx, offset, color, pos, atten[0], atten[1], atten[2]);
			m_scene.m_Lights.push_back(light);
		}
		else if (header == "SpotLight:")
		{
			std::cout << "SpotLight:=====================================\n";
			int offset;
			{
				std::getline(sceneFile, line);
				offset = Tools::parseInt(line);
			}
			glm::vec3 pos;
			{
				std::getline(sceneFile, line);
				pos = Tools::parseVec3(line);
			}
			glm::vec3 color;
			{
				std::getline(sceneFile, line);
				color = Tools::parseVec3(line);
			}
			glm::vec3 dir;
			{
				std::getline(sceneFile, line);
				dir = Tools::parseVec3(line);
			}
			glm::vec2 angleCoe;
			{
				std::getline(sceneFile, line);
				angleCoe = Tools::parseVec2(line);
			}
			glm::vec3 atten;
			{
				std::getline(sceneFile, line);
				atten = Tools::parseVec3(line);
			}
			std::shared_ptr<SpotLight> light = std::make_shared<SpotLight>(gfx, offset, color, pos, dir, angleCoe, atten);
			m_scene.m_Lights.push_back(light);
		}
		else if (header == "DirectionalLight:")
		{
			std::cout << "DirectionalLight:=====================================\n";
			int offset;
			{
				std::getline(sceneFile, line);
				offset = Tools::parseInt(line);
			}
			glm::vec3 dir;
			{
				std::getline(sceneFile, line);
				dir = Tools::parseVec3(line);
			}

			glm::vec3 color;
			{
				std::getline(sceneFile, line);
				color = Tools::parseVec3(line);
			}
			std::shared_ptr<DirectionalLight> light = std::make_shared<DirectionalLight>(gfx, offset, color, dir);
			m_scene.m_Lights.push_back(light);
		}
		else if (header == "Entity:")
		{
			std::vector<std::shared_ptr<Bindable>> binds;
			std::cout << "Entity:=========================================\n";
			std::getline(sceneFile, line);
			std::string path = Tools::parseStr(line);

			//VertexShader
			std::getline(sceneFile, line);
			binds.push_back(BindableFactory::CreateBindable(gfx, BVertexShader, { line }));

			//PixelShader
			std::getline(sceneFile, line);
			binds.push_back(BindableFactory::CreateBindable(gfx, BPixelShader, { line }));

			//Sampler
			std::getline(sceneFile, line);
			binds.push_back(BindableFactory::CreateBindable(gfx, BSampler, { line }));

			//VSConstantBuffer
			glm::mat4 transform = glm::mat4(1.0);
			std::getline(sceneFile, line);
			glm::vec3 translate = Tools::parseVec3(line);
			std::getline(sceneFile, line);
			glm::vec4 rotate = Tools::parseVec4(line);
			std::getline(sceneFile, line);
			glm::vec3 scale = Tools::parseVec3(line);
			transform = glm::scale(transform, scale);
			transform = glm::rotate(transform, glm::radians(rotate.x), glm::vec3(rotate.y, rotate.z, rotate.w));
			transform = glm::translate(transform, translate);

			//Topology
			std::getline(sceneFile, line);

			binds.push_back(BindableFactory::CreateBindable(gfx, BTopology, { line }));
			//RenderState
			std::vector<std::string> content;
			std::getline(sceneFile, line);
			content.push_back(line);
			std::getline(sceneFile, line);
			content.push_back(line);
			binds.push_back(BindableFactory::CreateBindable(gfx, BRenderStates, content));

			std::getline(sceneFile, line);
			binds.push_back(BindableFactory::CreateBindable(gfx, BBlendState, { line }));
			std::getline(sceneFile, line);
			bool hasLight = Tools::parseBool(line);
			m_scene.m_Models.push_back({ std::make_shared<Model>(gfx, binds, path.c_str(), transform), hasLight });
		}
		else if (header == "Plane:")
		{
			std::vector<std::shared_ptr<Bindable>> binds;
			std::cout << "Plane:=========================================\n";


			//VertexShader
			std::getline(sceneFile, line);
			binds.push_back(BindableFactory::CreateBindable(gfx, BVertexShader, { line }));

			//PixelShader
			std::getline(sceneFile, line);
			binds.push_back(BindableFactory::CreateBindable(gfx, BPixelShader, { line }));

			//Texture
			std::vector<std::string> content;
			std::getline(sceneFile, line);
			content.push_back(line);
			std::getline(sceneFile, line);
			content.push_back(line);
			std::getline(sceneFile, line);
			content.push_back(line);
			binds.push_back(BindableFactory::CreateBindable(gfx, BTexture2D, content));

			//Sampler
			std::getline(sceneFile, line);
			binds.push_back(BindableFactory::CreateBindable(gfx, BSampler, { line }));

			//VSConstantBuffer
			glm::mat4 transform = glm::mat4(1.0);
			std::getline(sceneFile, line);
			glm::vec3 translate = Tools::parseVec3(line);
			std::getline(sceneFile, line);
			glm::vec4 rotate = Tools::parseVec4(line);
			std::getline(sceneFile, line);
			glm::vec3 scale = Tools::parseVec3(line);
			transform = glm::scale(transform, scale);
			transform = glm::rotate(transform, glm::radians(rotate.x), glm::vec3(rotate.y, rotate.z, rotate.w));
			transform = glm::translate(transform, translate);

			//RenderState
			content.clear();
			std::getline(sceneFile, line);
			content.push_back(line);
			std::getline(sceneFile, line);
			content.push_back(line);
			binds.push_back(BindableFactory::CreateBindable(gfx, BRenderStates, content));

			std::getline(sceneFile, line);
			binds.push_back(BindableFactory::CreateBindable(gfx, BBlendState, { line }));
			std::getline(sceneFile, line);
			bool hasLight = Tools::parseBool(line);
			m_scene.m_Entities.push_back({ std::make_shared<Plane>(gfx, binds, transform), hasLight });
		}
	}
}