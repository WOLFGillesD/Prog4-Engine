#pragma once

#include "Component.h"
#include "Command.h"
#include "DaeTime.h"
#include "Event.h"
#include "States.h"
#include "GridComponent.h"
#include "HealthComponent.h"
#include "Renderer.h"
#include "ResourceManager.h"
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
        PlayerComponent(dae::GameObject& go, dae::ColliderComponent* pCollider ,GridComponent* pGrid, MovementComponent* pMovementComponent, dae::TextComponent* pTxtComp, game::ScoreComponent* pScoreComponent, HealthComponent* pHealth, dae::Scene* pScene)
            : Component(go)
			, movementState{ pMovementComponent }
            , m_pGrid{ pGrid }
			, m_ScoreObserver(std::make_unique<game::ScoreObserver>(pTxtComp))
			, m_pHealthComp(pHealth)
            , m_pScene(pScene)
			, m_pCollider(pCollider)
        {
            pHealth->GetOnDieEvent()->AddObserver(&m_HealthObserver);
            ChangeState(&movementState);
			pScoreComponent->OnScoreChanged()->AddObserver(m_ScoreObserver.get());
            m_pCollider->SetCollisionCallback([this](dae::ColliderComponent& other) { OnCollide(other); });
        }

        void ChangeState(IState* newState);
        void Update() override;

    	MoveState   movementState;
        IState*     currentState{ nullptr };

        void OnHealthChanged(int newHealth);
        void OnCollide(dae::ColliderComponent& other);
    private:

		std::unique_ptr<ScoreObserver> m_ScoreObserver;

        dae::ColliderComponent* m_pCollider;
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
	    PlayerLivesComponent(dae::GameObject& go, HealthComponent* pHealth, const std::string& path, float scale = 1.f)
		    : Component(go)
			, m_pHealthComp(pHealth)
			, m_Scale(scale)
	    {
            m_Texture2D = dae::ResourceManager::GetInstance().LoadTexture(path);
            pHealth->GetOnDieEvent()->AddObserver(&m_healthObserver);
            m_CurrentLives = pHealth->GetLives();
	    }

        void UpdateLives(int count);
        
        Observer<int> m_healthObserver{ this, &PlayerLivesComponent::UpdateLives};

        void Render() const override;

    private:
        HealthComponent* m_pHealthComp;
        std::shared_ptr<dae::Texture2D> m_Texture2D;

        int m_CurrentLives;
        float m_Scale;
    };

}
