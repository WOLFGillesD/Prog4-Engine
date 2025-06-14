#include "GameManager.h"

#include <unordered_set>

#include "SceneManager.h"

namespace game
{
	void GamemanagerComponent::TransitionScene() const
	{
		static const std::unordered_set<std::string> targetNames = {
				"Level 1", "Level 2", "Level 3"
		};

		std::string contains{};
		for (const auto& name: targetNames)
		{
			if (dae::SceneManager::GetInstance().ContainsScene(name))
				contains = name;
		}

		if (!contains.empty())
		{
			dae::SceneManager::GetInstance().TransitionScene(contains, m_Newscene);
		}
	}

	void GamemanagerComponent::RegisterEmerald(EmeraldComponent* pEmerald)
	{
		m_pEmeralds.push_back(pEmerald);
	}

	void GamemanagerComponent::RemoveEmerald(EmeraldComponent* pEmerald)
	{
		if (!m_pEmeralds.empty())
		{
			m_pEmeralds.erase(
				std::remove(m_pEmeralds.begin(), m_pEmeralds.end(), pEmerald),
				m_pEmeralds.end()
			);
		}
		if (m_pEmeralds.empty() && !m_MarkedForRemoval)
			TransitionScene();
	}
}
