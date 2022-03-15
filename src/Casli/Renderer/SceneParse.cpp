#include "SceneParse.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <Model.h>
#include <DirectionalLight.h>
#include <PointLight.h>
#include <SpotLight.h>
#include <glm/gtc/matrix_transform.hpp>


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
				m_scene.m_CameraPos = parseVec3(line);
			}

			{
				std::getline(sceneFile, line);
				m_scene.m_CameraFront = parseVec3(line);
			}

			{
				std::getline(sceneFile, line);
				m_scene.m_CameraUp = parseVec3(line);
			}


		}
		else if (header == "Frustum:")
		{
			std::cout << "Frustum:========================================\n";
			{
				std::getline(sceneFile, line);
				m_scene.m_FrustumFovy = parseFloat(line);
			}

			{
				std::getline(sceneFile, line);
				m_scene.m_FrustumNear = parseFloat(line);
			}

			{
				std::getline(sceneFile, line);
				m_scene.m_FrustumFar = parseFloat(line);
			}
		}
		else if (header == "PointLight:")
		{
			std::cout << "PointLight:=====================================\n";
			unsigned int offset;
			{
				std::getline(sceneFile, line);
				offset = parseFloat(line);
			}
			glm::vec3 pos;
			{
				std::getline(sceneFile, line);
				pos = parseVec3(line);
			}

			glm::vec3 atten;
			{
				std::getline(sceneFile, line);
				atten = parseVec3(line);
			}

			glm::vec3 color;
			{
				std::getline(sceneFile, line);
				color = parseVec3(line);
			}
			std::shared_ptr<PointLight> point = std::make_shared<PointLight>(gfx, color, pos, atten[0], atten[1], atten[2], offset);
			m_scene.m_lights.push_back(point);
		}
		else if (header == "SpotLight:")
		{
			std::cout << "SpotLight:=====================================\n";
			glm::vec3 pos;
			{
				std::getline(sceneFile, line);
				pos = parseVec3(line);
			}

			glm::vec3 atten;
			{
				std::getline(sceneFile, line);
				atten = parseVec3(line);
			}

			glm::vec3 color;
			{
				std::getline(sceneFile, line);
				color = parseVec3(line);
			}

			float innerCutoff;
			{
				std::getline(sceneFile, line);
				innerCutoff = parseFloat(line);
			}

			float outerCutoff;
			{
				std::getline(sceneFile, line);
				outerCutoff = parseFloat(line);
			}

			glm::vec3 spotDir;
			{
				std::getline(sceneFile, line);
				spotDir = parseVec3(line);
			}

		}
		else if (header == "DirectionalLight:")
		{
			std::cout << "DirectionalLight:=====================================\n";
			glm::vec3 dir;
			{
				std::getline(sceneFile, line);
				dir = parseVec3(line);
			}

			glm::vec3 color;
			{
				std::getline(sceneFile, line);
				color = parseVec3(line);
			}
		}
		else if (header == "Entity:")
		{
			std::cout << "Entity:=========================================\n";
			std::getline(sceneFile, line);
			std::string path = parseStr(line);
			std::shared_ptr<Drawable> drawable = std::make_shared<Model>(gfx, path.c_str(), 8);
			m_scene.m_entities.push_back(drawable);
			{
				glm::vec3 translate;
				{
					std::getline(sceneFile, line);
					translate = parseVec3(line);
				}

				glm::vec4 rotation;
				{
					std::getline(sceneFile, line);
					rotation = parseVec4(line);
				}

				glm::vec3 scale;
				{
					std::getline(sceneFile, line);
					scale = parseVec3(line);
				}
				glm::mat4 modelMatrix(1.0f);
				modelMatrix = glm::scale(modelMatrix, scale);
				modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.x), glm::vec3(rotation.y, rotation.z, rotation.w));
				modelMatrix = glm::translate(modelMatrix, translate);
				drawable->SetModelMatrix(modelMatrix);
			}
			{
				std::getline(sceneFile, line);
				std::string cullface = parseStr(line);
				if (cullface == "back")
					drawable->SetCullFaceMode(CullFaceMode::CULL_BACK);
				else if (cullface == "front")
					drawable->SetCullFaceMode(CullFaceMode::CULL_FRONT);
				else
					drawable->SetCullFaceMode(CullFaceMode::CULL_DISABLE);
			}
			{
				std::getline(sceneFile, line);
				std::string alphablend = parseStr(line);
				if (alphablend == "true")
				{
					drawable->SetBlendState(true);
				}
				else
				{
					drawable->SetBlendState(false);
				}
			}
			{
				for (int i = 0; i < 7; i++)
					std::getline(sceneFile, line);
			}
		}
	}
}

float SceneParse::parseFloat(std::string str) const
{
	std::stringstream ss;
	std::string token;
	ss << str;
	ss >> token;
	float ret;
	ss >> ret;
	std::cout << token << " " << ret << std::endl;
	return ret;
}

glm::vec3 SceneParse::parseVec3(std::string str) const
{
	std::stringstream ss;
	std::string token;
	ss << str;
	ss >> token;
	glm::vec3 ret;
	ss >> ret.x;
	ss >> ret.y;
	ss >> ret.z;
	std::cout << token << " " << ret.x << "," << ret.y << "," << ret.z << std::endl;
	return ret;
}

glm::vec4 SceneParse::parseVec4(std::string str) const
{
	std::stringstream ss;
	std::string token;
	ss << str;
	ss >> token;
	glm::vec4 ret;
	ss >> ret.x;
	ss >> ret.y;
	ss >> ret.z;
	ss >> ret.w;
	std::cout << token << " " << ret.x << "," << ret.y << "," << ret.z << std::endl;
	return ret;
}

bool SceneParse::parseBool(std::string str) const
{
	std::stringstream ss;
	std::string token;
	ss << str;
	ss >> token;
	bool ret = true;
	std::string tag;
	ss >> tag;
	if (tag == "true")
		ret = true;
	if (tag == "false")
		ret = false;
	std::cout << token << " " << (ret ? "true" : "false") << std::endl;
	return true;
}

std::string SceneParse::parseStr(std::string str) const
{
	std::stringstream ss;
	std::string token;
	ss << str;
	ss >> token;
	std::string ret;
	ss >> ret;
	std::cout << token << " " << ret << std::endl;
	return ret;
}
