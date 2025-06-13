#pragma once
#include <functional>
#include <vector>
#include "GameObject.h"

template<typename... Args>
class BaseObserver
{
public:
	virtual ~BaseObserver() = default;
	virtual void Trigger(Args... args) = 0;
private:
};

template<typename... Args>
class Observer final : public BaseObserver<Args...>
{
public:
	Observer(const std::function<void(Args...)>& callback)
	{
		m_Callback = callback;
	}
	
	template<typename ObjType>
	Observer(ObjType* obj, void (ObjType::* func)(Args...))
	{
		if (obj == nullptr)
			return;

		m_Callback = [obj, func](Args... args)
		{
			(obj->*func)(args...);
		};
	}
	
	template<typename ObjType>
	Observer(ObjType* obj, void (ObjType::* func)(Args...) const)
	{
		if (obj == nullptr)
			return;

		m_Callback = [obj, func](Args... args)
			{
				(obj->*func)(args...);
			};
	}

	virtual void Trigger(Args... args) override
	{
		m_Callback(args...);
	}

	virtual ~Observer() = default;

private:
	std::function<void(Args...)> m_Callback;
};

template<typename... Args>
class Event
{
public:
	Event() = default;
	~Event() = default;

	Event(const Event& other) = delete;
	Event(Event&& other) noexcept = delete;
	Event& operator=(const Event& other) = delete;
	Event& operator=(Event&& other) noexcept = delete;

	void operator+=(BaseObserver<Args...>* observer) { AddObserver(observer); }
	void operator-=(BaseObserver<Args...>* observer) { RemoveObserver(observer); }
	void operator()(Args... args) { Trigger(args...); }

	void AddObserver(BaseObserver<Args...>* observer)
	{
		if (!observer) return;
		m_Observers.push_back(observer);
	}

	void RemoveObserver(BaseObserver<Args...>* observer)
	{
		if (!observer) return;
		if (m_Observers.empty()) return;

		std::erase(m_Observers, observer);
	}

	void RemoveAllListeners()
	{
		m_Observers.clear();
	}

	void Trigger(Args... args)
	{
		for (const auto& observer : m_Observers)
			observer->Trigger(args...);
	}

private:
	std::vector<BaseObserver<Args...>*> m_Observers{};
};
