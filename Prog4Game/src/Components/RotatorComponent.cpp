#include "RotatorComponent.h"

#include "DaeTime.h"
#include "GameObject.h"

dae::RotatorComponent::RotatorComponent(GameObject& go, float radius, float rotationSpeed)
	: Component(go), m_radius(radius), m_rotationSpeed(rotationSpeed)
{
}

void dae::RotatorComponent::Update()
{
	m_CurrentRot += Time::m_DeltaTime * m_rotationSpeed;
	if (m_CurrentRot >= 2.f*M_PI)
	{
		m_CurrentRot -= static_cast<float>(2.f*M_PI);
	}
	glm::vec2 pos{};
	pos.x += m_radius * cos(m_CurrentRot);
	pos.y += m_radius * sin(m_CurrentRot);

	GetOwner()->SetLocalPosition(pos.x, pos.y);
}
