#pragma once
#include "Component.h"
#include "Command.h"
#include "Event.h"
#include "SceneManager.h"
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

	class MainScreenUIManagerComponent final : public dae::Component
	{
	public:
		MainScreenUIManagerComponent(dae::GameObject& go, const std::function<void()>& func1, const std::function<void()>& func2, const std::function<void()>& func3)
			: Component(go)
			, m_F1(func1)
			, m_F2(func2)
			, m_F3(func3)
		{
		}

		Observer<>* GetPVEObserver() { return &m_PVEObserver; }
		Observer<>* GetPVPObserver() { return &m_PVPObserver; }
		Observer<>* GetCoOpObserver() { return &m_CObserver; }

		void OnPVEPress() const
		{
			dae::SceneManager::GetInstance().TransitionScene("Main menu", m_F1);
		}

		void OnPVPPress()
		{
			//dae::SceneManager::GetInstance().TransitionScene("Main menu", m_F2);

		}
		void OnCoOpPress()
		{
			dae::SceneManager::GetInstance().TransitionScene("Main menu", m_F3);
		}
	private:

		std::function<void()> m_F1;
		std::function<void()> m_F2;
		std::function<void()> m_F3;

		Observer<> m_PVEObserver{ this, &MainScreenUIManagerComponent::OnPVEPress };
		Observer<> m_PVPObserver{ this, &MainScreenUIManagerComponent::OnPVEPress };
		Observer<> m_CObserver{ this, &MainScreenUIManagerComponent::OnCoOpPress };
	};

}
