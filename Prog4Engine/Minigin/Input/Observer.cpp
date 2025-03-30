#include "Observer.h"

void Event::AddObserver(Observer* observer)
{
	m_observers.push_back(observer);
}

void Event::RemoveObserver(Observer* observer)
{
	m_observers.erase(std::remove(m_observers.begin(), m_observers.end(), observer));
}

void Event::Invoke(dae::GameObject* actor) const
{
	for (const auto& observer : m_observers)
	{
		observer->OnTrigger(actor);
	}
}
