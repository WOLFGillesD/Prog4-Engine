#pragma once
#include <glm.hpp>

namespace dae
{
	class Transform final
	{
	public:
		const glm::vec3& GetPosition() const { return m_position; }
		void SetPosition(float x, float y, float z);
		void SetPosition(glm::vec3 newPos);

	private:
		glm::vec3 m_position{};
	};
}
