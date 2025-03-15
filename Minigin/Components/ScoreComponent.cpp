#include "ScoreComponent.h"

#include "TextComponent.h"

#include <string>

dae::ScoreComponent::ScoreComponent(dae::GameObject& go)
	: Component(go)
{
}

void dae::ScoreComponent::AddScore(int amount)
{
	m_score += amount;
	m_OnScoreChange->Invoke(GetOwner());
}

void dae::ScoreComponent::RemoveScore(int amount)
{
	m_score -= amount;
	m_OnScoreChange->Invoke(GetOwner());
}

int dae::ScoreComponent::GetScore() const
{
	return m_score;
}

Event* dae::ScoreComponent::OnScoreChanged() const
{
	return m_OnScoreChange.get();
}

dae::ScoreObserver::ScoreObserver(TextComponent* txtComponent)
	: m_TextIndicator(txtComponent)
{
}

void dae::ScoreObserver::OnTrigger(GameObject* actor)
{
	m_TextIndicator->SetText("Score: " + std::to_string(actor->GetComponent<ScoreComponent>()->GetScore()));
}

dae::UpScoreCommand::UpScoreCommand(ScoreComponent* sc, int scoreIncrease)
	: m_sc(sc)
	, m_scoreIncrease(scoreIncrease)
{
}

void dae::UpScoreCommand::Execute()
{
	m_sc->AddScore(m_scoreIncrease);
}
