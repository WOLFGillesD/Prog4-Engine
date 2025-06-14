#include "NPCComponents.h"

#include <array>

#include "TextureComponent.h"

namespace game
{
	void Nobbin::Update()
	{
        // Get current cell and player cell
        glm::vec2 pos = GetOwner()->GetWorldPosition();
        glm::ivec2 current = m_pGrid->GetCell(pos);
        glm::vec2 playerPos = m_pPlayer->GetOwner()->GetWorldPosition();
        glm::ivec2 playerCell = m_pGrid->GetCell(playerPos);

        // Build candidate directions: forward, left, right
        std::array<glm::ivec2, 3> dirs = { m_Direction, RotateLeft(m_Direction), RotateRight(m_Direction) };
        glm::ivec2 bestDir{ 0,0 };
        float bestDistSq = std::numeric_limits<float>::infinity();

        // Test each
        for (auto d : dirs)
        {
            glm::ivec2 next = current + d;
            if (!m_pGrid->IsCellValid(next) || m_pGrid->IsObstacle(next)) continue;
            // distance squared to player
            float dx = float(next.x - playerCell.x);
            float dy = float(next.y - playerCell.y);
            float distSq = dx * dx + dy * dy;
            if (distSq < bestDistSq)
            {
                bestDistSq = distSq;
                bestDir = d;
            }
        }

        // If none valid, reverse
        if (bestDir.x == 0 && bestDir.y == 0)
        {
            bestDir = -m_Direction;
            glm::ivec2 rev = current + bestDir;
            if (!m_pGrid->IsCellValid(rev) || m_pGrid->IsObstacle(rev))
            {
                // stuck: do nothing
                return;
            }
        }

        m_Direction = bestDir;
        // Issue movement command
        m_pMovementComponent->HandleInput(glm::vec2(float(m_Direction.x), float(m_Direction.y)));
	}

	void Nobbin::OnDeath() const
	{
		m_pScoreComponent->AddScore(m_Score);
		GetOwner()->SetMarkForRemoval();
	}

    Hobbin::Hobbin(dae::GameObject& go
        , GridComponent* pGrid
        , MovementComponent* pMovementComponent
        , PlayerComponent* pPlayer
        , ScoreComponent* pScoreComponent
        , HealthComponent* pHealth)
        : Component(go)
        , m_pGrid(pGrid)
        , m_pMovementComponent(pMovementComponent)
        , m_pPlayer(pPlayer)
        , m_pScoreComponent(pScoreComponent)
        , m_pHealthComp(pHealth)
        , m_HealthObserver{ this, &Hobbin::OnDeath }
    {
        pHealth->GetOnOutOfLivesEvent()->AddObserver(&m_HealthObserver);
    }

    void Hobbin::Update()
    {
        // Determine current and player cells
        glm::vec2 pos = GetOwner()->GetWorldPosition();
        glm::ivec2 current = m_pGrid->GetCell(pos);
        glm::vec2 playerPos = m_pPlayer->GetOwner()->GetWorldPosition();
        glm::ivec2 playerCell = m_pGrid->GetCell(playerPos);

        // Evaluate forward, left, right
        std::array<glm::ivec2, 3> candidates = { m_Direction, RotateLeft(m_Direction), RotateRight(m_Direction) };
        glm::ivec2 bestDir{ 0,0 };
        float bestDistSq = std::numeric_limits<float>::infinity();

        for (auto& d : candidates)
        {
            glm::ivec2 next = current + d;
            if (!m_pGrid->IsCellValid(next)) continue;
            float dx = float(next.x - playerCell.x);
            float dy = float(next.y - playerCell.y);
            float distSq = dx * dx + dy * dy;
            if (distSq < bestDistSq)
            {
                bestDistSq = distSq;
                bestDir = d;
            }
        }

        // If no candidate found, reverse direction
        if (bestDir == glm::ivec2{ 0,0 })
        {
            bestDir = -m_Direction;
            glm::ivec2 rev = current + bestDir;
            if (!m_pGrid->IsCellValid(rev) || m_pGrid->IsObstacle(rev))
                return; // stuck
        }

        // Commit direction
        m_Direction = bestDir;
        glm::ivec2 target = current + m_Direction;

        // If terrain, dig; else just move
        if (m_pGrid->IsDirt(target))
        {
            m_pGrid->Dig(target);
        }

        // Issue movement
        glm::vec2 input{ float(m_Direction.x), float(m_Direction.y) };
        m_pMovementComponent->HandleInput(input);
    }

    void Hobbin::OnDeath() const
    {
        m_pScoreComponent->AddScore(m_Score);
        GetOwner()->SetMarkForRemoval();
    }

    EnemySpawner::EnemySpawner(dae::GameObject& go,
        GridComponent* pGrid,
        PlayerComponent* pPlayer,
        ScoreComponent* pScore,
        dae::Scene* pScene,
        float spawnInterval,
        int maxEnemies,
        float hobbinChance)
        : Component(go)
        , m_pGrid(pGrid)
        , m_pPlayer(pPlayer)
        , m_pScore(pScore)
        , m_SpawnInterval(spawnInterval)
        , m_MaxEnemies(maxEnemies)
        , m_HobbinChance(hobbinChance)
        , m_Rng(std::random_device{}())
        , m_ChanceDist(0.f, 1.f)
		, m_pScene(pScene)
    {
    }

    void EnemySpawner::Update()
    {
        m_ElapsedTime += dae::Time::m_DeltaTime;
        // Remove dead count? Could listen to death events to decrement m_ActiveCount.

        if (m_ActiveCount >= m_MaxEnemies) return;
        if (m_ElapsedTime < m_SpawnInterval) return;

        m_ElapsedTime = 0.f;
        SpawnEnemy();
        m_ActiveCount++;
    }

    void EnemySpawner::SpawnEnemy()
    {
        float roll = m_ChanceDist(m_Rng);

        if (roll < m_HobbinChance)
        {
			auto npc1 = std::make_shared<dae::GameObject>();
			npc1->AddComponent<game::MovementComponent>(m_pGrid, 100.f, glm::ivec2{ 14,0 });

			npc1->AddComponent<dae::SpriteComponent>("VRHOB1.png", 1, 1, 0, 0.f, false);
			npc1->AddComponent<dae::ColliderComponent>(glm::vec2(40, 40), glm::vec2(0, 0), "Enemy");
			npc1->AddComponent<game::HealthComponent>();

			npc1->AddComponent<game::Hobbin>(m_pGrid
				, npc1->GetComponent<game::MovementComponent>()
				, m_pPlayer
				, m_pScore
				, npc1->GetComponent<game::HealthComponent>());

			m_pScene->Add(npc1);
        }
        else
        {
            auto npc1 = std::make_shared<dae::GameObject>();
            npc1->AddComponent<game::MovementComponent>(m_pGrid, 100.f, glm::ivec2{ 14,0 });

            npc1->AddComponent<dae::SpriteComponent>("VNOBD.png", 1, 1, 0, 0.f, false);
            npc1->AddComponent<dae::ColliderComponent>(glm::vec2(40, 40), glm::vec2(0, 0), "Enemy");
            npc1->AddComponent<game::HealthComponent>();

            npc1->AddComponent<game::Nobbin>(m_pGrid
                , npc1->GetComponent<game::MovementComponent>()
                , m_pPlayer
                , m_pScore
                , npc1->GetComponent<game::HealthComponent>());

            m_pScene->Add(npc1);
        }
        // Add to scene
    }
}
