#pragma once

#include "Component.h"
#include "Command.h"
#include "DaeTime.h"
#include "Event.h"
#include "States.h"
#include "GridComponent.h"
#include "HealthComponent.h"
#include "Renderer.h"
#include "Scene.h"
#include "ScoreComponent.h"
#include "TextComponent.h"

namespace game
{
	class MovementComponent final : public dae::Component
	{
    public:
        MovementComponent(dae::GameObject& go, GridComponent* grid, float speed, const glm::ivec2& startCell = { 0,0 }, bool canDig = true);

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
        bool m_IsMoving{ false };
        bool m_CanDig{ true };
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
        PlayerComponent(dae::GameObject& go, GridComponent* pGrid, MovementComponent* pMovementComponent, dae::TextComponent* pTxtComp, game::ScoreComponent* pScoreComponent, HealthComponent* pHealth, dae::Scene* pScene)
            : Component(go)
			, movementState{ pMovementComponent }
            , m_pGrid{ pGrid }
			, m_ScoreObserver(std::make_unique<game::ScoreObserver>(pTxtComp))
			, m_pHealthComp(pHealth)
            , m_pScene(pScene)
        {
            pHealth->GetOnDieEvent()->AddObserver(&m_HealthObserver);
            ChangeState(&movementState);
			pScoreComponent->OnScoreChanged()->AddObserver(m_ScoreObserver.get());
        }

        void ChangeState(IState* newState);
        void Update() override;

    	MoveState   movementState;
        IState*     currentState{ nullptr };

        void OnHealthChanged(int newHealth);
    private:

		std::unique_ptr<ScoreObserver> m_ScoreObserver;

        GridComponent* m_pGrid;
		MovementComponent* m_pMovementComponent;
        HealthComponent* m_pHealthComp;
        dae::Scene* m_pScene;

        Observer<int> m_HealthObserver{this, &PlayerComponent::OnHealthChanged};

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

    class PlayerLivesComponent : public dae::Component
    {
    public:
	    PlayerLivesComponent(dae::GameObject& go, HealthComponent* pHealth)
		    : Component(go)
			, m_pHealthComp(pHealth)
	    {

	    }

        void Render() const override;

    private:
        HealthComponent* m_pHealthComp;

    };

}
