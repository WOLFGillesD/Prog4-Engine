#pragma once
#include "Component.h"
#include "Input/Command.h"

namespace dae
{
	class ControllerMovementComponent;

	class MoveCommand : public Command
	{
		ControllerMovementComponent* m_actor;
	protected:
		ControllerMovementComponent* GetGameActor() const { return m_actor; }
	public:
		MoveCommand(ControllerMovementComponent* actor);

		MoveCommand(const MoveCommand& other) = delete;
		MoveCommand(MoveCommand&& other) noexcept = delete;
		MoveCommand& operator=(const MoveCommand& other) = delete;
		MoveCommand& operator=(MoveCommand&& other) noexcept = delete;

		~MoveCommand() override;
	};

	class MoveLeftCommand final : public MoveCommand
	{
	public:
		MoveLeftCommand(ControllerMovementComponent* actor)
			: MoveCommand(actor)
		{
		}

		void Execute() override;
	};

	class MoveRightCommand final : public MoveCommand
	{
	public:
		MoveRightCommand(ControllerMovementComponent* actor)
			: MoveCommand(actor)
		{
		}

		void Execute() override;
	};

	class MoveUpCommand final : public MoveCommand
	{
	public:
		MoveUpCommand(ControllerMovementComponent* actor)
			: MoveCommand(actor)
		{
		}

		void Execute() override;
	};

	class MoveDownCommand final : public MoveCommand
	{
	public:
		MoveDownCommand(ControllerMovementComponent* actor)
			: MoveCommand(actor)
		{
		}

		void Execute() override;
	};

	class ControllerMovementComponent : public Component
	{
		float m_moveSpeed;

		MoveLeftCommand m_MoveLeft{ this };
		MoveRightCommand m_MoveRight{ this };
		MoveUpCommand m_MoveUp{ this };
		MoveDownCommand m_MoveDown{ this };

	public:
		ControllerMovementComponent(GameObject& go, float moveSpeed = 25.f);

		ControllerMovementComponent(const ControllerMovementComponent& other) = delete;
		ControllerMovementComponent(ControllerMovementComponent&& other) noexcept = delete;
		ControllerMovementComponent& operator=(const ControllerMovementComponent& other) = delete;
		ControllerMovementComponent& operator=(ControllerMovementComponent&& other) noexcept = delete;

		void OnMoveLeft() const;
		void OnMoveRight() const;
		void OnMoveUp() const;
		void OnMoveDown() const;
	};
}
