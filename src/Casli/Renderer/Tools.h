#include <string>
#include <sstream>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <assimp/Importer.hpp>

namespace Tools
{
	extern int parseInt(std::string str);

	extern float parseFloat(std::string str);

	extern glm::vec2 parseVec2(std::string str);

	extern glm::vec3 parseVec3(std::string str);

	extern glm::vec4 parseVec4(std::string str);

	extern bool parseBool(std::string str);

	extern std::string parseStr(std::string str);

	extern void CopyaiMat(const aiMatrix4x4 *from, glm::mat4 &to);
}
