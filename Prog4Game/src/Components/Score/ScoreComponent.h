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

		std::unique_ptr<Event<int>> m_OnScoreChange{ std::make_unique<Event<int>>() };
	public:
		ScoreComponent(dae::GameObject& go);
		void AddScore(int amount);
		void RemoveScore(int amount);

		int GetScore() const;
		Event<int>* OnScoreChanged() const;
	};

	class ScoreObserver : public BaseObserver<int>
	{
		dae::TextComponent* m_TextIndicator{};
	public:
		ScoreObserver(dae::TextComponent* txtComponent);

		void Trigger(int newScore) override;
	};

	//class UpScoreCommand : public dae::Command
	//{
	//	ScoreComponent* m_sc;
	//	int m_scoreIncrease;

	//public:
	//	UpScoreCommand(ScoreComponent* sc, int scoreIncrease);

	//	void Execute() override;
	//};


	class DiamondComponent : public dae::Component
	{
	public:
		DiamondComponent(dae::GameObject& go, dae::ColliderComponent* pCollider, ScoreComponent* pScoreComponent);
		~DiamondComponent() override = default;

		DiamondComponent(const DiamondComponent&) = delete;
		DiamondComponent(DiamondComponent&&) noexcept = delete;
		DiamondComponent& operator=(const DiamondComponent&) = delete;
		DiamondComponent& operator=(DiamondComponent&&) noexcept = delete;

		ScoreComponent* m_pScore;

		void Update() override;

	private:
		const int m_ScoreValue{ 25 };

		dae::ColliderComponent* m_pCollider;
		void OnCollide(dae::ColliderComponent& other);
	};

	class BagComponent : public dae::Component
	{
	public:
		enum class State { Static, Delaying, Falling, Pickupable, Destroyed };

		BagComponent(dae::GameObject& go,
			GridComponent* grid,
			dae::ColliderComponent* pCollider,
			ScoreComponent* pScoreComponent = nullptr,
			float fallDelay = 1.f);
		~BagComponent() override = default;

		BagComponent(const BagComponent&) = delete;
		BagComponent(BagComponent&&) noexcept = delete;
		BagComponent& operator=(const BagComponent&) = delete;
		BagComponent& operator=(BagComponent&&) noexcept = delete;

		void Update() override;

		static Event<dae::GameObject&>& OnBagFall();
		static Event<dae::GameObject&>& OnBagPickup();

	private:
		const int m_ScoreValue{ 500 };

		dae::ColliderComponent* m_Collider;
		GridComponent* m_pGrid;
		ScoreComponent* m_pScore;

		State       m_State{ State::Static };
		glm::ivec2  m_Cell{};
		glm::ivec2  m_StartFallCell{};
		float       m_FallDelay{};
		float       m_DelayTimer{};

		void CheckGridBelow();
		void StartDelay();
		void StartFalling();
		void Land();
		void OnCollide(dae::ColliderComponent& other);
	};
}
