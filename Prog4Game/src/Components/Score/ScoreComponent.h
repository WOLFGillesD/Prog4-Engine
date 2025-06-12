#pragma once
#include <memory>

#include "ColliderComponent.h"
#include "Component.h"
#include "Input/Command.h"
#include "Event.h"
#include "GridComponent.h"
#include "TextComponent.h"

namespace game
{
	class ScoreComponent : public dae::Component
	{
		int m_score{};

		std::unique_ptr<Event<dae::GameObject*>> m_OnScoreChange{ std::make_unique<Event<dae::GameObject*>>() };
	public:
		ScoreComponent(dae::GameObject& go);
		void AddScore(int amount);
		void RemoveScore(int amount);

		int GetScore() const;
		Event<dae::GameObject*>* OnScoreChanged() const;
	};

	class ScoreObserver : public BaseObserver<dae::GameObject*>
	{
		dae::TextComponent* m_TextIndicator{};
	public:
		ScoreObserver(dae::TextComponent* txtComponent);

		void Trigger(dae::GameObject* actor) override;
	};

	class UpScoreCommand : public dae::Command
	{
		ScoreComponent* m_sc;
		int m_scoreIncrease;

	public:
		UpScoreCommand(ScoreComponent* sc, int scoreIncrease);

		void Execute() override;
	};


	class DiamondComponent : public dae::Component
	{
	public:
		DiamondComponent(dae::GameObject& go, dae::ColliderComponent* pCollider);
		~DiamondComponent() override = default;

		DiamondComponent(const DiamondComponent&) = delete;
		DiamondComponent(DiamondComponent&&) noexcept = delete;
		DiamondComponent& operator=(const DiamondComponent&) = delete;
		DiamondComponent& operator=(DiamondComponent&&) noexcept = delete;

		void Update() override;

	private:
		dae::ColliderComponent* m_pCollider;
		void OnCollide(dae::ColliderComponent& other);
	};

	class BagComponent : public dae::Component
	{
	public:
		enum class State { Static, Falling, Pickupable, Destroyed };

		BagComponent(dae::GameObject& go, GridComponent& grid, dae::ColliderComponent* pCollider);
		~BagComponent() override = default;

		BagComponent(const BagComponent&) = delete;
		BagComponent(BagComponent&&) noexcept = delete;
		BagComponent& operator=(const BagComponent&) = delete;
		BagComponent& operator=(BagComponent&&) noexcept = delete;

		void Update() override;

		// Optionally expose bag events
		static Event<dae::GameObject&>& OnBagFall();
		static Event<dae::GameObject&>& OnBagPickup();

	private:
		dae::ColliderComponent* m_Collider;
		GridComponent& m_Grid;
		State m_State{ State::Static };
		glm::ivec2 m_Cell;

		void CheckGridBelow();
		void StartFalling();
		void Land();
		void OnCollide(dae::ColliderComponent& other);
	};
}
