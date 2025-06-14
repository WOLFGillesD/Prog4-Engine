#pragma once
#include "Component.h"
#include "Command.h"
#include "Event.h"
#include "TextComponent.h"

namespace game
{
	class UIButtonComponent final : public dae::Component
	{
	public:
		UIButtonComponent(dae::GameObject& go, const glm::ivec2& size, const glm::ivec2& offset = {0,0})
			: Component(go)
			, m_Size(size)
			, m_Offset(offset)
		{}

		void ShowBackground(bool shouldShow) { m_ShowBg = shouldShow; }

		void Render() const override;

		void Press();

		Event<>* GetOnButtonPress() { return &m_OnButtonPress; }

	private:
		Event<> m_OnButtonPress;

		glm::ivec2 m_Size{};
		glm::ivec2 m_Offset{};

		bool m_ShowBg{ false };
	};

	class UIControllerComponent : public dae::Component
	{
	public:
		UIControllerComponent(dae::GameObject& go)
			: Component(go)
		{}

		void Move(int dir);
		void Press() const;

		void RegisterButton(UIButtonComponent* btn);

	private:
		std::vector<UIButtonComponent*> m_KnownButtons{};
		int m_CurrentIndex{};
	};

	class UIMoveUICommand final : public dae::Command
	{
	public:
		UIMoveUICommand(UIControllerComponent* uiController, int dir)
			: m_dir(dir)
			, m_UIC(uiController)
		{}

		void Execute() override;
	private:
		int m_dir;
		UIControllerComponent* m_UIC;
	};

	class UIPressCommand final : public dae::Command
	{
	public:
		UIPressCommand(UIControllerComponent* uiController)
			: m_UIC(uiController)
		{
		}

		void Execute() override;
	private:
		UIControllerComponent* m_UIC;

	};
}
