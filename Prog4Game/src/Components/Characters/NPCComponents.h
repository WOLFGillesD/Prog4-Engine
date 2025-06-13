#pragma once
#include <memory>

#include "Component.h"
#include "HealthComponent.h"
#include "PlayerComponent.h"
#include "ScoreComponent.h"

namespace game
{
	class Nobbin final : public dae::Component
    {
    public:
        Nobbin(dae::GameObject& go, GridComponent* pGrid, MovementComponent* pMovementComponent, PlayerComponent* pPlayer, ScoreComponent* pScoreComponent, HealthComponent* pHealth)
            : Component(go)
            , m_pGrid{ pGrid }
			, m_pMovementComponent(pMovementComponent)
            , m_pHealthComp(pHealth)
			, m_pScoreComponent(pScoreComponent)
			, m_pPlayer(pPlayer)
            , m_Direction{ -1,0 }
        {
            pHealth->GetOnOutOfLivesEvent()->AddObserver(&m_HealthObserver);
        }

        void Update() override;

        void OnDeath() const;

    private:
        static glm::ivec2 RotateLeft(const glm::ivec2& d) { return glm::ivec2{ d.y, -d.x }; }
        static glm::ivec2 RotateRight(const glm::ivec2& d) { return glm::ivec2{ -d.y, d.x }; }


        const int m_Score{ 250 };

        GridComponent* m_pGrid{};
        MovementComponent* m_pMovementComponent{};
        ScoreComponent* m_pScoreComponent{};
        HealthComponent* m_pHealthComp{};
        PlayerComponent* m_pPlayer{};

        Observer<> m_HealthObserver{ this, &Nobbin::OnDeath };

        glm::vec2 m_Direction{};
    };

    class Hobbin final : public dae::Component
    {
    public:
        Hobbin(dae::GameObject& go
            , GridComponent* pGrid
            , MovementComponent* pMovementComponent
            , PlayerComponent* pPlayer
            , ScoreComponent* pScoreComponent
            , HealthComponent* pHealth);
        ~Hobbin() override = default;

        void Update() override;
        void OnDeath() const;

    private:
        static glm::ivec2 RotateLeft(const glm::ivec2& d) { return glm::ivec2{ d.y, -d.x }; }
        static glm::ivec2 RotateRight(const glm::ivec2& d) { return glm::ivec2{ -d.y, d.x }; }

        const int m_Score{ 250 };

        GridComponent* m_pGrid;
        MovementComponent* m_pMovementComponent;
        PlayerComponent* m_pPlayer;
        ScoreComponent* m_pScoreComponent;
        HealthComponent* m_pHealthComp;
        Observer<> m_HealthObserver;

        glm::ivec2 m_Direction{ 1,0 };  // initial direction right
    };
}
