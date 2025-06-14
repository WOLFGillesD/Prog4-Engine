#include "ScoreComponent.h"

#include <iostream>

#include "TextComponent.h"

#include <string>

#include "DaeTime.h"
#include "GameManager.h"
#include "HealthComponent.h"

game::ScoreComponent::ScoreComponent(dae::GameObject& go)
	: Component(go)
{
}

void game::ScoreComponent::AddScore(int amount)
{
	m_score += amount;
	m_OnScoreChange->Trigger(m_score);
}

void game::ScoreComponent::RemoveScore(int amount)
{
	m_score -= amount;
	m_OnScoreChange->Trigger(m_score);
}

int game::ScoreComponent::GetScore() const
{
	return m_score;
}

Event<int>* game::ScoreComponent::OnScoreChanged() const
{
	return m_OnScoreChange.get();
}

game::ScoreObserver::ScoreObserver(dae::TextComponent* txtComponent)
	: m_TextIndicator(txtComponent)
{
	txtComponent->SetText("Score: 0");
}

void game::ScoreObserver::Trigger(int newScore)
{
    m_TextIndicator->SetText("Score: " + std::to_string(newScore));
}

//game::UpScoreCommand::UpScoreCommand(ScoreComponent* sc, int scoreIncrease)
//	: m_sc(sc)
//	, m_scoreIncrease(scoreIncrease)
//{
//}
//
//void game::UpScoreCommand::Execute()
//{
//	m_sc->AddScore(m_scoreIncrease);
//}

game::EmeraldComponent::EmeraldComponent(dae::GameObject& go,dae::ColliderComponent* pCollider, ScoreComponent* pScoreComponent, GamemanagerComponent* pGamemanagerComponent)
	: Component(go)
	, m_pCollider(pCollider)
	, m_pScore(pScoreComponent)
	, m_pGamemanager(pGamemanagerComponent)
{
    pGamemanagerComponent->RegisterEmerald(this);
	m_pCollider->SetCollisionCallback([this](dae::ColliderComponent& other) { OnCollide(other); });
}

game::EmeraldComponent::~EmeraldComponent()
{
    m_pGamemanager->RemoveEmerald(this);
}

void game::EmeraldComponent::Update()
{
	
}

void game::EmeraldComponent::OnCollide(dae::ColliderComponent& other)
{
    if (other.GetTag() == "Player")
		m_pScore->AddScore(m_ScoreValue);
	GetOwner()->SetMarkForRemoval();
	std::cout << "Diamond collected by Player" << "\n";
}

Event<dae::GameObject&>& game::BagComponent::OnBagFall() { static Event<dae::GameObject&> e; return e; }
Event<dae::GameObject&>& game::BagComponent::OnBagPickup() { static Event<dae::GameObject&> e; return e; }

game::BagComponent::BagComponent(dae::GameObject& go,
                                 GridComponent* grid,
                                 dae::ColliderComponent* pCollider,
                                 ScoreComponent* pScoreComponent,
                                 float fallDelay)
    : Component(go)
    , m_pGrid(grid)
    , m_Collider(pCollider)
    , m_pScore(pScoreComponent)
    , m_FallDelay(fallDelay)
    , m_DelayTimer(0.0f)
{
    m_Collider->SetCollisionCallback([this](dae::ColliderComponent& other) { OnCollide(other); });
    m_Cell = m_pGrid->GetCell(GetOwner()->GetLocalPosition());
}

void game::BagComponent::Update()
{
    switch (m_State)
    {
    case State::Static:
        CheckGridBelow();
        break;

    case State::Delaying:
        m_DelayTimer -= dae::Time::m_DeltaTime;
        if (m_DelayTimer <= 0.0f)
        {
            StartFalling();
        }
        break;

    case State::Falling:
        GetOwner()->SetLocalPosition(glm::vec2{ GetOwner()->GetLocalPosition() } + glm::vec2{ 0, m_pGrid->GetCellSize() * 2 * dae::Time::m_DeltaTime });
        m_Cell = m_pGrid->GetCell(GetOwner()->GetLocalPosition());
        {
            glm::ivec2 below = m_Cell + glm::ivec2{ 0,1 };
            if (m_pGrid->IsCellValid(below) && m_pGrid->GetCellState(below.x, below.y) != GridComponent::Cell::State::Empty)
            {
                Land();
            }
            else if (!m_pGrid->IsCellValid(below))
            {
                Land();
            }
        }
        break;

    case State::Pickupable:
        break;

    case State::Destroyed:
        GetOwner()->SetMarkForRemoval();
        break;
    }
}

void game::BagComponent::CheckGridBelow()
{
    glm::ivec2 below = m_Cell + glm::ivec2{ 0,1 };
    if (m_pGrid->IsCellValid(below) && m_pGrid->GetCellState(below.x, below.y) == GridComponent::Cell::State::Empty)
    {
        StartDelay();
    }
}

void game::BagComponent::StartDelay()
{
    m_State = State::Delaying;
    m_DelayTimer = m_FallDelay;
    m_StartFallCell = m_Cell;
}

void game::BagComponent::StartFalling()
{
    m_State = State::Falling;
    OnBagFall().Trigger(*GetOwner());
}

void game::BagComponent::Land()
{
    int fallDistance = m_Cell.y - m_StartFallCell.y;
    if (fallDistance > 1)
    {
        m_State = State::Pickupable;
        OnBagPickup().Trigger(*GetOwner());
    }
    else
    {
        m_State = State::Static;
    }
}

void game::BagComponent::OnCollide(dae::ColliderComponent& other)
{
	switch (m_State)
	{
	case State::Static:
	    {
	        float dir = (other.GetOwner()->GetLocalPosition().x < GetOwner()->GetLocalPosition().x) ? 1.0f : -1.0f;
	        GetOwner()->SetLocalPosition(glm::vec2{ GetOwner()->GetLocalPosition() } + glm::vec2{ m_pGrid->GetCellSize() * dir, 0 });
	        m_Cell = m_pGrid->GetCell(GetOwner()->GetLocalPosition());
	    }
		break;
	case State::Pickupable:
	    {
			if (other.GetTag() == "Player")
			{
				m_pScore->AddScore(500);
	            GetOwner()->SetMarkForRemoval();
			}
	        else if (other.GetTag() == "Enemy")
	        {
	            GetOwner()->SetMarkForRemoval();
	        }
	    }
		break;
    default:
	case State::Delaying:
		break;
	case State::Falling:
		{
			auto hComp = other.GetOwner()->GetComponent<HealthComponent>();
            if (hComp == nullptr) break;

            hComp->Die();
		}
		break;
	case State::Destroyed:
		break;
	}
}
