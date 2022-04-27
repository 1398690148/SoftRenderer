#include <glm/vec3.hpp>
#include <vector>
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
		std::vector<std::pair<std::shared_ptr<Drawable>, bool>> m_Entities;
		std::vector< std::pair<std::shared_ptr<Model>, bool>> m_Models;
	} m_scene;
	void parse(const std::string &path, Graphics &gfx);
};