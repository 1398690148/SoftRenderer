#include "SceneParse.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <Model.h>
#include <glm/gtc/matrix_transform.hpp>

void SceneParse::parse(const std::string &path, Graphics & gfx, bool generatedMipmap)
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
		else if (header == "Entity:")
		{
			std::cout << "Entity:=========================================\n";
			std::string name;
			{
				std::getline(sceneFile, line);
				name = parseStr(line);
			}

			std::getline(sceneFile, line);
			std::string path = parseStr(line);
			std::shared_ptr<Drawable> drawable = std::make_shared<Model>(gfx, path.c_str(), 8);
			m_scene.m_entities[name] = drawable;
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
