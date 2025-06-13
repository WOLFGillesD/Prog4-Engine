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

		std::unique_ptr<Event<int>> m_OnDie{ std::make_unique<Event<int>>()};
		std::unique_ptr<Event<>> m_OutOfLives{ std::make_unique<Event<>>()};

	public:
		HealthComponent(GameObject& go, int startLives = 3, int maxLives = 5);

		Event<>* GetOnOutOfLivesEvent() const { return m_OutOfLives.get(); }
		Event<int>* GetOnDieEvent() const { return m_OnDie.get(); }

		void Die();
		void AddLive(int amount);

		int GetLives() const;
	};

	class HealthObserver : public BaseObserver<int>
	{
		TextComponent* m_TextIndicator{};
	public:
		HealthObserver(TextComponent* txtComponent);

		void Trigger(int newLifeCount) override;
	};

	class DieCommand : public Command
	{
		HealthComponent* m_hc;
	public:
		DieCommand(HealthComponent* hc);

		void Execute() override;
	};
}
