#include "KeyboardMovementComponent.h"

#include "DaeTime.h"
#include "GameObject.h"
#include "InputManager.h"
#include "SDL_keycode.h"

namespace dae
{
	KeyBoardMoveCommand::KeyBoardMoveCommand(KeyboardMovementComponent* actor)
		: m_actor(actor)
	{
	}

	KeyBoardMoveCommand::~KeyBoardMoveCommand()
	{
	}

	void KeyBoardMoveLeftCommand::Execute()
	{
		GetGameActor()->OnMoveLeft();
	}

	void KeyBoardMoveRightCommand::Execute()
	{
		GetGameActor()->OnMoveRight();
	}

	void KeyBoardMoveUpCommand::Execute()
	{
		GetGameActor()->OnMoveUp();
	}

	void KeyBoardMoveDownCommand::Execute()
	{
		GetGameActor()->OnMoveDown();
	}

	KeyboardMovementComponent::KeyboardMovementComponent(GameObject& go, float moveSpeed)
		: Component(go), m_moveSpeed(moveSpeed)
	{
		auto& inputManager = dae::InputManager::GetInstance();
		auto inputMapping = inputManager.GetKeyboardInputMapping();

		dae::InputCommand ic{ &m_MoveLeft, dae::InputState::IsPressed };
		inputMapping->AddCommand(static_cast<unsigned int>(SDLK_a), ic);
		ic = { &m_MoveRight, dae::InputState::IsPressed };
		inputMapping->AddCommand(static_cast<unsigned int>(SDLK_d), ic);
		ic = { &m_MoveUp, dae::InputState::IsPressed };
		inputMapping->AddCommand(static_cast<unsigned int>(SDLK_w), ic);
		ic = { &m_MoveDown, dae::InputState::IsPressed };
		inputMapping->AddCommand(static_cast<unsigned int>(SDLK_s), ic);

	}

	void KeyboardMovementComponent::OnMoveLeft() const
	{
		auto oldPos = GetOwner()->GetLocalPosition();
		GetOwner()->SetLocalPosition(oldPos.x - 1 * m_moveSpeed * Time::m_DeltaTime, oldPos.y);
	}

	void KeyboardMovementComponent::OnMoveRight() const
	{
		auto oldPos = GetOwner()->GetLocalPosition();
		GetOwner()->SetLocalPosition(oldPos.x + 1 * m_moveSpeed * Time::m_DeltaTime, oldPos.y);
	}

	void KeyboardMovementComponent::OnMoveUp() const
	{
		auto oldPos = GetOwner()->GetLocalPosition();
		GetOwner()->SetLocalPosition(oldPos.x, oldPos.y - 1 * m_moveSpeed * Time::m_DeltaTime);
	}

	void KeyboardMovementComponent::OnMoveDown() const
	{
		auto oldPos = GetOwner()->GetLocalPosition();
		GetOwner()->SetLocalPosition(oldPos.x, oldPos.y + 1 * m_moveSpeed * Time::m_DeltaTime);
	}
}
