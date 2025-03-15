#pragma once
#include "Singleton.h"
#include <vector>
#include "Observer.h"

namespace dae
{
	class EventManager : public Singleton<EventManager>
	{
		std::vector<std::unique_ptr<Observer>> m_Observers;
	public:
		void AddObserver(std::unique_ptr<Observer> observer)
		{
			m_Observers.emplace_back(std::move(observer));
		}

		void RemoveObserver(Observer*)
		{
			//m_Observers.erase(std::find(m_Observers.begin(), m_Observers.end(), observer));
		}

	};
}
