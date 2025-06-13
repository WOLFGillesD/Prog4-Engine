#include "HealthComponent.h"

#include <algorithm>

#include "TextComponent.h"

game::HealthComponent::HealthComponent(dae::GameObject& go,int startLives, int maxLives)
	: Component(go)
	, m_CurrentLives(startLives)
	, m_MaxLives(maxLives)
{
}

void game::HealthComponent::Die()
{
	if (m_CurrentLives <= 0)
	{
		m_OutOfLives->Trigger();
		return;
	}
	--m_CurrentLives;
	m_OnDie->Trigger(m_CurrentLives);
}

void game::HealthComponent::AddLive(int amount)
{
	m_CurrentLives += amount;
	m_CurrentLives = std::clamp(m_CurrentLives, 0, m_MaxLives);
}

int game::HealthComponent::GetLives() const
{
	return m_CurrentLives;
}

game::HealthObserver::HealthObserver(dae::TextComponent* txtComponent)
	: m_TextIndicator(txtComponent)
{
}

void game::HealthObserver::Trigger(int newLifeCount)
{
	m_TextIndicator->SetText("# Lives: " + std::to_string(newLifeCount));
}

game::DieCommand::DieCommand(HealthComponent* hc)
	:m_hc(hc)
{
}

void game::DieCommand::Execute()
{
	m_hc->Die();
}
