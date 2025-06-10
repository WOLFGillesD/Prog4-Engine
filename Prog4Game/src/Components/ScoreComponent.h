#pragma once
#include <memory>

#include "Component.h"
#include "Input/Command.h"
#include "Event.h"

namespace dae
{
	class TextComponent;

	class ScoreComponent : public dae::Component
	{
		int m_score{};

		std::unique_ptr<Event<GameObject*>> m_OnScoreChange{ std::make_unique<Event<GameObject*>>() };
	public:
		ScoreComponent(dae::GameObject& go);
		void AddScore(int amount);
		void RemoveScore(int amount);

		int GetScore() const;
		Event<GameObject*>* OnScoreChanged() const;
	};

	class ScoreObserver : public BaseObserver<GameObject*>
	{
		TextComponent* m_TextIndicator{};
	public:
		ScoreObserver(TextComponent* txtComponent);

		void Trigger(GameObject* actor) override;
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