#include "Command.h"

#include "DaeTime.h"

MoveCommand::MoveCommand(dae::GameObject& pActor, const glm::vec2& inputDirection, float speed)
	: m_pActor(&pActor)
	, m_InputDirection(inputDirection)
	, m_Speed(speed)
{
}

void MoveCommand::Execute()
{
	glm::vec2 oldPos{};
	oldPos.x = m_pActor->GetLocalPosition().x;
	oldPos.y = m_pActor->GetLocalPosition().y;

	auto newPos = oldPos + m_InputDirection * m_Speed * dae::Time::m_DeltaTime;

	m_pActor->SetLocalPosition(newPos.x, newPos.y);
}

void MoveCommand::SetInput(const glm::vec2& v2)
{
	m_InputDirection = v2;
}
