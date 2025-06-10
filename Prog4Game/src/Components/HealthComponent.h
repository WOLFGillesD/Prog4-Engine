#pragma once
#include "Component.h"
#include "Input/Command.h"
#include "Event.h"

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

		std::unique_ptr<Event<GameObject*>> m_OnDie{ std::make_unique<Event<GameObject*>>()};
		std::unique_ptr<Event<GameObject*>> m_OnDeath{ std::make_unique<Event<GameObject*>>()};

	public:
		HealthComponent(GameObject& go, int startLives = 3, int maxLives = 5);

		Event<GameObject*>* GetOnDeathEvent() const { return m_OnDeath.get(); }
		Event<GameObject*>* GetOnDieEvent() const { return m_OnDie.get(); }

		void Die();
		void AddLive(int amount);

		int GetLives();
	};

	class HealthObserver : public BaseObserver<GameObject*>
	{
		TextComponent* m_TextIndicator{};
	public:
		HealthObserver(TextComponent* txtComponent);

		void Trigger(GameObject* actor) override;
	};

	class DieCommand : public Command
	{
		HealthComponent* m_hc;
	public:
		DieCommand(HealthComponent* hc);

		void Execute() override;
	};
}
