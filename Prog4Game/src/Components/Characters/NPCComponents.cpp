#include "NPCComponents.h"

#include <array>

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
}
