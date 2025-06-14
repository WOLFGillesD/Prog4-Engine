#pragma once
#include <memory>
#include <random>

#include "Component.h"
#include "HealthComponent.h"
#include "PlayerComponent.h"
#include "Scene.h"
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

    class EnemySpawner final : public dae::Component
    {
    public:
        EnemySpawner(dae::GameObject& go,
            GridComponent* pGrid,
            PlayerComponent* pPlayer,
            ScoreComponent* pScore,
            dae::Scene* pScene,
            float spawnInterval = 5.f,
            int maxEnemies = 4,
            float hobbinChance = 0.1f);
        void Update() override;

    private:
        GridComponent* m_pGrid;
        PlayerComponent* m_pPlayer;
        ScoreComponent* m_pScore;
        dae::Scene* m_pScene;

        float m_SpawnInterval;
        int m_MaxEnemies;
        float m_HobbinChance;

        float m_ElapsedTime{ 0.f };
        int m_ActiveCount{ 0 };

        std::mt19937 m_Rng;
        std::uniform_real_distribution<float> m_ChanceDist;

        // Helpers
        void SpawnEnemy();
    };
}
