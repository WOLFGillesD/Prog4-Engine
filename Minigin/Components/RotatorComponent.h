#pragma once
#include "Component.h"
#include "SDL_stdinc.h"
#include "Transform.h"

namespace dae
{

	class RotatorComponent : public Component
	{
	public:
		RotatorComponent(GameObject& go, float radius = 25.f, float rotationSpeed = static_cast<float>(M_PI));
		~RotatorComponent() override = default;

		void Start() override;
		void Update() override;

		RotatorComponent(const RotatorComponent& other) = delete;
		RotatorComponent(RotatorComponent&& other) noexcept = delete;
		RotatorComponent& operator=(const RotatorComponent& other) = delete;
		RotatorComponent& operator=(RotatorComponent&& other) noexcept = delete;

	private:
		const float m_radius{};
		const float m_rotationSpeed{};
		float m_CurrentRot{};
		glm::vec2 m_center{};
	};
}
