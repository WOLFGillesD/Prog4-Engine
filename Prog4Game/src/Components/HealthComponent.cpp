#include "HealthComponent.h"

#include <algorithm>

#include "TextComponent.h"

dae::HealthComponent::HealthComponent(GameObject& go,int startLives, int maxLives)
	: Component(go)
	, m_CurrentLives(startLives)
	, m_MaxLives(maxLives)
{
}

void dae::HealthComponent::Die()
{
	if (m_CurrentLives <= 0)
	{
		m_OnDeath->Invoke(GetOwner());
		return;
	}
	--m_CurrentLives;
	m_OnDie->Invoke(GetOwner());
}

void dae::HealthComponent::AddLive(int amount)
{
	m_CurrentLives += amount;
	m_CurrentLives = std::clamp(m_CurrentLives, 0, m_MaxLives);
}

int dae::HealthComponent::GetLives()
{
	return m_CurrentLives;
}

dae::HealthObserver::HealthObserver(TextComponent* txtComponent)
	: m_TextIndicator(txtComponent)
{
}

void dae::HealthObserver::OnTrigger(GameObject* actor)
{
	m_TextIndicator->SetText("# Lives: " + std::to_string(actor->GetComponent<HealthComponent>()->GetLives()));
}

dae::DieCommand::DieCommand(HealthComponent* hc)
	:m_hc(hc)
{
}

void dae::DieCommand::Execute()
{
	m_hc->Die();
}
