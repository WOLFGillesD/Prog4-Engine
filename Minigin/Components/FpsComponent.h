#pragma once
#include "Component.h"
#include "TextComponent.h"

namespace dae
{

	class FpsComponent : public Component
	{
	public:
		FpsComponent(GameObject& go);
		~FpsComponent() override = default;

		void Start() override;
		void Update() override;

		FpsComponent(const FpsComponent& other) = delete;
		FpsComponent(FpsComponent&& other) noexcept = delete;
		FpsComponent& operator=(const FpsComponent& other) = delete;
		FpsComponent& operator=(FpsComponent&& other) noexcept = delete;

		TextComponent* m_TextComponent;
	};
}
