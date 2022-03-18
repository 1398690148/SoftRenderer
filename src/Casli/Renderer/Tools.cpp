#include "Tools.h"

namespace Tools
{
	int parseInt(std::string str)
	{
		std::stringstream ss;
		std::string token;
		ss << str;
		ss >> token;
		int ret;
		ss >> ret;
		std::cout << token << " " << ret << std::endl;
		return ret;
	}

	float parseFloat(std::string str)
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

	glm::vec2 parseVec2(std::string str)
	{
		std::stringstream ss;
		std::string token;
		ss << str;
		ss >> token;
		glm::vec2 ret;
		ss >> ret.x;
		ss >> ret.y;
		std::cout << token << " " << ret.x << "," << ret.y << std::endl;
		return ret;
	}

	glm::vec3 parseVec3(std::string str)
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

	glm::vec4 parseVec4(std::string str)
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

	bool parseBool(std::string str)
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
		return ret;
	}

	std::string parseStr(std::string str)
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
	void CopyaiMat(const aiMatrix4x4 *from, glm::mat4 &to)
	{
		to[0][0] = from->a1; to[1][0] = from->a2;
		to[2][0] = from->a3; to[3][0] = from->a4;
		to[0][1] = from->b1; to[1][1] = from->b2;
		to[2][1] = from->b3; to[3][1] = from->b4;
		to[0][2] = from->c1; to[1][2] = from->c2;
		to[2][2] = from->c3; to[3][2] = from->c4;
		to[0][3] = from->d1; to[1][3] = from->d2;
		to[2][3] = from->d3; to[3][3] = from->d4;
	}
}
