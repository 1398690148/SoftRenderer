#include <glm/vec3.hpp>
#include <map>
#include <Drawable.h>
#include <RendererAPI.h>
#include <memory>

class RENDERER_API SceneParse
{
public:

	struct Config
	{
		glm::vec3 m_CameraPos;
		glm::vec3 m_CameraFront;
		glm::vec3 m_CameraUp;

		float m_FrustumFovy;
		float m_FrustumNear;
		float m_FrustumFar;

		std::map<std::string, int> m_lights;
		std::map<std::string, std::shared_ptr<Drawable>> m_entities;
	} m_scene;
	void parse(const std::string &path, Graphics &gfx, bool generatedMipmap);

private:
	float parseFloat(std::string str) const;
	glm::vec3 parseVec3(std::string str) const;
	glm::vec4 parseVec4(std::string str) const;
	bool parseBool(std::string str) const;
	std::string parseStr(std::string str) const;
};