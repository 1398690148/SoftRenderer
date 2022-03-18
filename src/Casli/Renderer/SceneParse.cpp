#include "SceneParse.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <Model.h>
#include <DirectionalLight.h>
#include <PointLight.h>
#include <SpotLight.h>
#include <BindableFactory.h>
#include <Tools.h>
#include <glm/gtx/transform.hpp>

void SceneParse::parse(const std::string &path, Graphics &gfx, bool generatedMipmap)
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
			std::shared_ptr<PointLight> light = std::make_shared<PointLight>(gfx, color, pos, atten[0], atten[1], atten[2]);
			m_scene.m_lights.push_back(light);
		}
		else if (header == "SpotLight:")
		{
			std::cout << "SpotLight:=====================================\n";
			glm::vec3 pos;
			{
				std::getline(sceneFile, line);
				pos = Tools::parseVec3(line);
			}

			glm::vec3 atten;
			{
				std::getline(sceneFile, line);
				atten = Tools::parseVec3(line);
			}

			glm::vec3 color;
			{
				std::getline(sceneFile, line);
				color = Tools::parseVec3(line);
			}

			float innerCutoff;
			{
				std::getline(sceneFile, line);
				innerCutoff = Tools::parseFloat(line);
			}

			float outerCutoff;
			{
				std::getline(sceneFile, line);
				outerCutoff = Tools::parseFloat(line);
			}

			glm::vec3 spotDir;
			{
				std::getline(sceneFile, line);
				spotDir = Tools::parseVec3(line);
			}

		}
		else if (header == "DirectionalLight:")
		{
			std::cout << "DirectionalLight:=====================================\n";
			int offset = 0;
			{
				std::getline(sceneFile, line);
				offset = Tools::parseFloat(line);
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
			//std::shared_ptr<DirectionalLight> light = std::make_shared<DirectionalLight>(gfx, color, dir, offset);
			//m_scene.m_lights.push_back(light);
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

			m_scene.m_entities.push_back(std::make_shared<Model>(gfx, binds, path.c_str(), transform));
			{
				std::getline(sceneFile, line);
				std::string cullface = Tools::parseStr(line);
				if (cullface == "back")
				{

				}
				else if (cullface == "front")
				{

				}
				else
				{

				}
			}
		}
	}
}