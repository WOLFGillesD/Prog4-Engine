#pragma once
#include <vector>

#include "Component.h"
#include "ScoreComponent.h"
#include "Audio/Servicelocator.h"

namespace game
{
	class GamemanagerComponent final : public dae::Component
	{
	public:
		GamemanagerComponent(dae::GameObject& go, const std::function<void()>& sceneToLoad, int soundId)
			: Component(go)
			, m_Newscene(sceneToLoad)
			, m_MenuSound(soundId)
		{
			m_MenuSound.Trigger();
		}

		void TransitionScene() const;

		void RegisterEmerald(EmeraldComponent* pEmerald);
		void RemoveEmerald(EmeraldComponent* pEmerald);

	private:
		std::function<void()> m_Newscene;

		std::vector<EmeraldComponent*> m_pEmeralds{};
		dae::SoundObserver m_MenuSound;
	};
}
