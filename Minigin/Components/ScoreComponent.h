#pragma once
#include <memory>

#include "Component.h"
#include "Input/Command.h"
#include "Input/Observer.h"

namespace dae
{
	class TextComponent;

	class ScoreComponent : public dae::Component
	{
		int m_score{};

		std::unique_ptr<Event> m_OnScoreChange{ std::make_unique<Event>() };
	public:
		ScoreComponent(dae::GameObject& go);
		void AddScore(int amount);
		void RemoveScore(int amount);

		int GetScore() const;
		Event* OnScoreChanged() const;
	};

	class ScoreObserver : public Observer
	{
		TextComponent* m_TextIndicator{};
	public:
		ScoreObserver(TextComponent* txtComponent);

		void OnTrigger(GameObject* actor) override;
	};

	class ScoreAchievement : public Observer
	{
	public:
		ScoreAchievement() = default;
		void OnTrigger(GameObject* actor) override;
	};

	class UpScoreCommand : public Command
	{
		ScoreComponent* m_sc;
		int m_scoreIncrease;

	public:
		UpScoreCommand(ScoreComponent* sc, int scoreIncrease);

		void Execute() override;
	};
}