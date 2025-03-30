#pragma once
#include "Component.h"
#include "Input/Command.h"
#include "Input/Observer.h"

namespace dae
{
	class TextComponent;
}

namespace dae
{

	class HealthComponent : public Component
	{
		int m_CurrentLives;
		int m_MaxLives;

		std::unique_ptr<Event> m_OnDie{ std::make_unique<Event>()};
		std::unique_ptr<Event> m_OnDeath{ std::make_unique<Event>() };

	public:
		HealthComponent(GameObject& go, int startLives = 3, int maxLives = 5);

		Event* GetOnDeathEvent() const { return m_OnDeath.get(); }
		Event* GetOnDieEvent() const { return m_OnDie.get(); }

		void Die();
		void AddLive(int amount);

		int GetLives();
	};

	class HealthObserver : public Observer
	{
		TextComponent* m_TextIndicator{};
	public:
		HealthObserver(TextComponent* txtComponent);

		void OnTrigger(GameObject* actor) override;
	};

	class DieCommand : public Command
	{
		HealthComponent* m_hc;
	public:
		DieCommand(HealthComponent* hc);

		void Execute() override;
	};
}
