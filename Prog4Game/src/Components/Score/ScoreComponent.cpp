#include "ScoreComponent.h"

#include <iostream>

#include "TextComponent.h"

#include <string>

game::ScoreComponent::ScoreComponent(dae::GameObject& go)
	: Component(go)
{
}

void game::ScoreComponent::AddScore(int amount)
{
	m_score += amount;
	m_OnScoreChange->Trigger(GetOwner());
}

void game::ScoreComponent::RemoveScore(int amount)
{
	m_score -= amount;
	m_OnScoreChange->Trigger(GetOwner());
}

int game::ScoreComponent::GetScore() const
{
	return m_score;
}

Event<dae::GameObject*>* game::ScoreComponent::OnScoreChanged() const
{
	return m_OnScoreChange.get();
}

game::ScoreObserver::ScoreObserver(dae::TextComponent* txtComponent)
	: m_TextIndicator(txtComponent)
{
}

void game::ScoreObserver::Trigger(dae::GameObject* actor)
{
	m_TextIndicator->SetText("Score: " + std::to_string(actor->GetComponent<ScoreComponent>()->GetScore()));
}

game::UpScoreCommand::UpScoreCommand(ScoreComponent* sc, int scoreIncrease)
	: m_sc(sc)
	, m_scoreIncrease(scoreIncrease)
{
}

void game::UpScoreCommand::Execute()
{
	m_sc->AddScore(m_scoreIncrease);
}

game::DiamondComponent::DiamondComponent(dae::GameObject& go,dae::ColliderComponent* pCollider)
	: Component(go)
	, m_pCollider(pCollider)
{
	m_pCollider->SetCollisionCallback([this](dae::ColliderComponent& other) { OnCollide(other); });
}

void game::DiamondComponent::Update()
{
	
}

void game::DiamondComponent::OnCollide(dae::ColliderComponent& /*other*/)
{
	// Fire score event and destroy self
	//g_ScoreEvent(100);  // Trigger event with 100 points
	GetOwner()->SetMarkForRemoval();
	std::cout << "Diamond collected by Player" << "\n";
}

Event<dae::GameObject&>& game::BagComponent::OnBagFall() { static Event<dae::GameObject&> e; return e; }
Event<dae::GameObject&>& game::BagComponent::OnBagPickup() { static Event<dae::GameObject&> e; return e; }

game::BagComponent::BagComponent(dae::GameObject& go, GridComponent& grid, dae::ColliderComponent* pCollider)
    : Component(go)
    , m_Grid(grid)
	, m_Collider(pCollider)
{
    m_Collider->SetCollisionCallback([this](dae::ColliderComponent& other) { OnCollide(other); });
    m_Cell = m_Grid.GetCell(GetOwner()->GetLocalPosition());
}

void game::BagComponent::Update()
{
    switch (m_State)
    {
    case State::Static:
        CheckGridBelow();
        break;
    case State::Falling:
        GetOwner()->SetLocalPosition(glm::vec2{ GetOwner()->GetLocalPosition() } + glm::vec2{ 0, 2 });
        m_Cell = m_Grid.GetCell(GetOwner()->GetLocalPosition());
        if (m_Grid.IsCellValid(m_Cell + glm::ivec2{ 0,1 }) &&
            m_Grid.GetCellState(m_Cell.x, m_Cell.y + 1) != GridComponent::Cell::State::Empty)
        {
            Land();
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
    auto below = m_Cell + glm::ivec2{ 0,1 };
    if (m_Grid.IsCellValid(below) && m_Grid.GetCellState(below.x, below.y) == GridComponent::Cell::State::Empty)
    {
        StartFalling();
    }
}

void game::BagComponent::StartFalling()
{
    m_State = State::Falling;
    std::cout << "Bag started falling\n";
    OnBagFall().Trigger(*GetOwner());  // Notify listeners
}

void game::BagComponent::Land()
{
    m_State = State::Static;
    std::cout << "Bag landed\n";
}

void game::BagComponent::OnCollide(dae::ColliderComponent& other)
{
    if (m_State == State::Falling)
    {
        m_State = State::Pickupable;
        OnBagFall().Trigger(*GetOwner());
    }
    else if (m_State == State::Pickupable)
    {
        m_State = State::Destroyed;
    }
    else if (m_State == State::Static)
    {
        // Simple side push
        auto dir = (other.GetOwner()->GetLocalPosition().x < GetOwner()->GetLocalPosition().x) ? 1 : -1;
        GetOwner()->SetLocalPosition(glm::vec2{ GetOwner()->GetLocalPosition() } + glm::vec2{ 16 * dir, 0 });
        m_Cell = m_Grid.GetCell(GetOwner()->GetLocalPosition());
        std::cout << "Bag pushed\n";
    }
}
