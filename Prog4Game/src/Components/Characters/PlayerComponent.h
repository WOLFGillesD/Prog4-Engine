#pragma once

#include "Component.h"
#include "Command.h"
#include "DaeTime.h"
#include "States.h"
#include "GridComponent.h"
#include "Renderer.h"

namespace game
{
	class PlayerComponent;

	class MovementComponent final : public dae::Component
	{
    public:
        MovementComponent(dae::GameObject& go, GridComponent* grid, float speed);

        void Update() override;

        void HandleInput(const glm::vec2& input);
        bool IsCellWalkable(const glm::ivec2& cell) const;

		float GetSpeed() const { return m_Movespeed; }

    private:
        GridComponent* m_pGrid{};

        glm::ivec2 m_CurrentCell{};
        glm::ivec2 m_TargetCell{};
        glm::vec2 m_Velocity{};

        float m_Movespeed{};
        bool isMoving{ false };
    };

	// +----------------------------------------+
	// |               States		            |
	// +----------------------------------------+

    class MoveState : public IState
    {
    public:
        MoveState(MovementComponent* pMovement)
            : m_pMovement(pMovement)
        {

        }

        void OnEnter() override;
        void OnExit() override;
        IState* Update() override;

    private:
		MovementComponent* m_pMovement{ nullptr };
    };

    class PlayerComponent final : public dae::Component
    {
    public:
        PlayerComponent(dae::GameObject& go, GridComponent* pGrid, MovementComponent* pMovementComponent)
            : Component(go)
			, movementState{ pMovementComponent }
            , m_pGrid{ pGrid }
        {
            ChangeState(&movementState);
        }

        void ChangeState(IState* newState);
        void Update() override;
        void Render() const override
        {
            SDL_SetRenderDrawColor(dae::Renderer::GetInstance().GetSDLRenderer(), 0, 255, 255, 255);

            SDL_Rect rect{ static_cast<int>(GetOwner()->GetWorldPosition().x), static_cast<int>(GetOwner()->GetWorldPosition().y), 20, 20};
            SDL_RenderFillRect(dae::Renderer::GetInstance().GetSDLRenderer(), &rect);
        }

    	MoveState   movementState;
        IState*     currentState{ nullptr };
		MovementComponent* m_pMovementComponent;
    private:

        GridComponent* m_pGrid;
    };

    class MoveCommand final : public dae::Command
	{
		MovementComponent* m_pMovementComponent;
		glm::vec2 m_InputDirection{};
	public:
		MoveCommand(MovementComponent* pMovementComponent, const glm::vec2& inputDirection);

		void Execute() override;
        void SetInput(const glm::vec2& v2) { m_InputDirection = v2; }
	};

}
