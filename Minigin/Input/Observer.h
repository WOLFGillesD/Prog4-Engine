#pragma once
#include <vector>
#include "GameObject.h"


class Observer
{
public:
	virtual ~Observer() = default;
	virtual void OnTrigger(dae::GameObject* actor) = 0;
};

class Event
{
public:
	void AddObserver(Observer* observer);
	void RemoveObserver(Observer* observer);

	void Invoke(dae::GameObject* actor) const;

private:
	std::vector<Observer*> m_observers{};
};
