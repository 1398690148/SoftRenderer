#include <glm/vec3.hpp>
#include <vector>
#include <memory>
#include <Drawable.h>

class Light;
class Graphics;
class Model;

class SceneParse
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

		std::vector<std::shared_ptr<Light>> m_Lights;
		std::vector<std::shared_ptr<Drawable>> m_Entities;
		std::vector<std::shared_ptr<Model>> m_Models;
	} m_scene;
	void parse(const std::string &path, Graphics &gfx, bool generatedMipmap);
};