#pragma once
#include "Component.h"
#include "Input/Command.h"

namespace dae
{
	class KeyboardMovementComponent;
	class KeyBoardMoveCommand : public Command
	{
		KeyboardMovementComponent* m_actor;
	protected:
		KeyboardMovementComponent* GetGameActor() const { return m_actor; }
	public:
		KeyBoardMoveCommand(KeyboardMovementComponent* actor);

		KeyBoardMoveCommand(const KeyBoardMoveCommand& other) = delete;
		KeyBoardMoveCommand(KeyBoardMoveCommand&& other) noexcept = delete;
		KeyBoardMoveCommand& operator=(const KeyBoardMoveCommand& other) = delete;
		KeyBoardMoveCommand& operator=(KeyBoardMoveCommand&& other) noexcept = delete;

		~KeyBoardMoveCommand() override;
	};

	class KeyBoardMoveLeftCommand final : public KeyBoardMoveCommand
	{
	public:
		KeyBoardMoveLeftCommand(KeyboardMovementComponent* actor)
			: KeyBoardMoveCommand(actor)
		{
		}

		void Execute() override;
	};

	class KeyBoardMoveRightCommand final : public KeyBoardMoveCommand
	{
	public:
		KeyBoardMoveRightCommand(KeyboardMovementComponent* actor)
			: KeyBoardMoveCommand(actor)
		{
		}

		void Execute() override;
	};

	class KeyBoardMoveUpCommand final : public KeyBoardMoveCommand
	{
	public:
		KeyBoardMoveUpCommand(KeyboardMovementComponent* actor)
			: KeyBoardMoveCommand(actor)
		{
		}

		void Execute() override;
	};

	class KeyBoardMoveDownCommand final : public KeyBoardMoveCommand
	{
	public:
		KeyBoardMoveDownCommand(KeyboardMovementComponent* actor)
			: KeyBoardMoveCommand(actor)
		{
		}

		void Execute() override;
	};

	class KeyboardMovementComponent : public Component
	{
		float m_moveSpeed;

		KeyBoardMoveLeftCommand m_MoveLeft{ this };
		KeyBoardMoveRightCommand m_MoveRight{ this };
		KeyBoardMoveUpCommand m_MoveUp{ this };
		KeyBoardMoveDownCommand m_MoveDown{ this };

	public:
		KeyboardMovementComponent(GameObject& go, float moveSpeed = 25.f);

		KeyboardMovementComponent(const KeyboardMovementComponent& other) = delete;
		KeyboardMovementComponent(KeyboardMovementComponent&& other) noexcept = delete;
		KeyboardMovementComponent& operator=(const KeyboardMovementComponent& other) = delete;
		KeyboardMovementComponent& operator=(KeyboardMovementComponent&& other) noexcept = delete;

		void OnMoveLeft() const;
		void OnMoveRight() const;
		void OnMoveUp() const;
		void OnMoveDown() const;
	};
}
