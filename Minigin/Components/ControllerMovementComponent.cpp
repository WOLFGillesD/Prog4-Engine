#include "ControllerMovementComponent.h"

#include "DaeTime.h"
#include "GameObject.h"
#include "InputManager.h"

namespace dae
{
	MoveCommand::MoveCommand(ControllerMovementComponent* actor)
		: m_actor(actor)
	{
	}

	MoveCommand::~MoveCommand()
	{
	}

	void MoveLeftCommand::Execute()
	{
		GetGameActor()->OnMoveLeft();
	}

	void MoveRightCommand::Execute()
	{
		GetGameActor()->OnMoveRight();
	}

	void MoveUpCommand::Execute()
	{
		GetGameActor()->OnMoveUp();
	}

	void MoveDownCommand::Execute()
	{
		GetGameActor()->OnMoveDown();
	}

	ControllerMovementComponent::ControllerMovementComponent(GameObject& go, float moveSpeed)
		: Component(go), m_moveSpeed(moveSpeed)
	{
		auto& inputManager = dae::InputManager::GetInstance();
		auto inputMapping = inputManager.GetInputMapping();

		dae::InputCommand ic{ &m_MoveLeft, dae::InputState::IsPressed };
		inputMapping->AddCommand(static_cast<int>(GamePadInput::GAMEPAD_DPAD_LEFT), ic);
		ic = { &m_MoveRight, dae::InputState::IsPressed };
		inputMapping->AddCommand(static_cast<int>(GamePadInput::GAMEPAD_DPAD_RIGHT), ic);
		ic = { &m_MoveUp, dae::InputState::IsPressed };
		inputMapping->AddCommand(static_cast<int>(GamePadInput::GAMEPAD_DPAD_UP), ic);
		ic = { &m_MoveDown, dae::InputState::IsPressed };
		inputMapping->AddCommand(static_cast<int>(GamePadInput::GAMEPAD_DPAD_DOWN), ic);

	}

	void ControllerMovementComponent::OnMoveLeft() const
	{
		auto oldPos = GetOwner()->GetLocalPosition();
		GetOwner()->SetLocalPosition(oldPos.x - 1 * m_moveSpeed * Time::m_DeltaTime, oldPos.y);
	}

	void ControllerMovementComponent::OnMoveRight() const
	{
		auto oldPos = GetOwner()->GetLocalPosition();
		GetOwner()->SetLocalPosition(oldPos.x + 1 * m_moveSpeed * Time::m_DeltaTime, oldPos.y);
	}

	void ControllerMovementComponent::OnMoveUp() const
	{
		auto oldPos = GetOwner()->GetLocalPosition();
		GetOwner()->SetLocalPosition(oldPos.x, oldPos.y - 1 * m_moveSpeed * Time::m_DeltaTime);
	}

	void ControllerMovementComponent::OnMoveDown() const
	{
		auto oldPos = GetOwner()->GetLocalPosition();
		GetOwner()->SetLocalPosition(oldPos.x, oldPos.y + 1 * m_moveSpeed * Time::m_DeltaTime);
	}

}
