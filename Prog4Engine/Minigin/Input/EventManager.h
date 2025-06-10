#pragma once
#include "Singleton.h"
#include <vector>
#include "Event.h"

namespace dae
{
	template<typename... Args>
	class EventManager : public Singleton<EventManager<Args>>
	{
		std::vector<std::unique_ptr<BaseObserver<Args...>>> m_Observers;
	public:
		void AddObserver(std::unique_ptr<BaseObserver<Args...>> observer)
		{
			m_Observers.emplace_back(std::move(observer));
		}

		void RemoveObserver(BaseObserver<Args...>* observer)
		{
			m_Observers.erase(std::find(m_Observers.begin(), m_Observers.end(), observer));
		}

	};
}
