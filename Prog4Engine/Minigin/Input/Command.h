#pragma once
#include <unordered_map>

#include "GameObject.h"
#include "vec2.hpp"

class Command
{
public:
	virtual ~Command() = default;
	virtual void Execute() = 0;
};

class MoveCommand final : public Command
{
	dae::GameObject* m_pActor;
	glm::vec2 m_InputDirection{};
	float m_Speed{};
public:
	MoveCommand(dae::GameObject& pActor,const glm::vec2& inputDirection, float speed = 25.f);

	void Execute() override;
	void SetInput(const glm::vec2& v2);
};