#include "UIControllerComponent.h"

#include "Renderer.h"
#include "SDL_render.h"

namespace game
{
	void UIButtonComponent::Render() const
	{
		if (!m_ShowBg) return;
		glm::ivec2 pos = glm::ivec2{ GetOwner()->GetWorldPosition() } + m_Offset;
		auto rect = SDL_Rect{ pos.x, pos.y , m_Size.x, m_Size.y };

		SDL_SetRenderDrawColor(dae::Renderer::GetInstance().GetSDLRenderer(), 170, 85, 0, 127);
		SDL_RenderFillRect(dae::Renderer::GetInstance().GetSDLRenderer(), &rect);
	}

	void UIButtonComponent::Press()
	{
		m_OnButtonPress.Trigger();
	}

	void UIControllerComponent::Press() const
	{
		if (m_CurrentIndex >= m_KnownButtons.size()) return;
		m_KnownButtons[m_CurrentIndex]->Press();
	}

	void UIControllerComponent::RegisterButton(UIButtonComponent* btn)
	{
		if (m_KnownButtons.empty()) btn->ShowBackground(true);
		m_KnownButtons.push_back(btn);
	}

	void UIControllerComponent::Move(int dir)
	{
		int newIndex = m_CurrentIndex;
		if (dir > 0 && m_CurrentIndex > 0)
		{
			--newIndex;
		}
		else if (dir < 0 && m_CurrentIndex < m_KnownButtons.size() - 1)
		{
			++newIndex;
		}

		m_KnownButtons[m_CurrentIndex]->ShowBackground(false);
		m_KnownButtons[newIndex]->ShowBackground(true);

		m_CurrentIndex = newIndex;
	}

	void UIMoveUICommand::Execute()
	{
		m_UIC->Move(m_dir);
	}

	void UIPressCommand::Execute()
	{
 		m_UIC->Press();
	}
}
