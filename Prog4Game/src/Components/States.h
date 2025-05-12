#pragma once
#include <unordered_map>
#include <memory>
#include <string>

namespace game
{
	class IState
	{
	public:
		IState() = default;
		virtual ~IState() = default;

		virtual void OnEnter() = 0;
		virtual void OnExit() = 0;
		virtual void Update() = 0;
	};

}

//namespace game
//{
//	class IBlackboardField
//	{
//	public:
//		IBlackboardField() = default;
//		virtual ~IBlackboardField() = default;
//	};
//
//	template<typename T>
//	class SMDataField : public IBlackboardField
//	{
//	public:
//		explicit SMDataField(T data) : m_Data(data)
//		{}
//
//		T GetData() const {	return m_Data; }
//		void SetData(T data) { m_Data = data; }
//
//	private:
//		T m_Data;
//	};
//
//	class SMData final
//	{
//	public:
//		SMData() = default;
//		~SMData()
//		{
//			for (auto& pair : m_Data)
//			{
//				pair.second.reset();
//			}
//			m_Data.clear();
//		}
//
//		SMData(const SMData& other) = delete;
//		SMData(SMData&& other) noexcept
//		{
//			m_Data = std::move(other.m_Data);
//			other.m_Data.clear();
//		}
//		SMData& operator=(const SMData& other) = delete;
//		SMData& operator=(SMData&& other) noexcept
//		{
//			if (this != &other)
//			{
//				m_Data = std::move(other.m_Data);
//				other.m_Data.clear();
//			}
//			return *this;
//		}
//
//		template<typename T> bool AddData(const std::string& key, T data)
//		{
//			if (m_Data.find(key) != m_Data.end())
//			{
//				return false;
//			}
//			m_Data[key] = std::make_unique<T>(data);
//			return true;
//		}
//
//		template<typename T> bool RemoveData(const std::string& key)
//		{
//			auto it = m_Data.find(key);
//			if (it != m_Data.end())
//			{
//				m_Data.erase(it);
//				return true;
//			}
//			return false;
//		}
//
//		template<typename T> bool GetData(const std::string& key, T& data) const
//		{
//			auto it = m_Data.find(key);
//			if (it != m_Data.end())
//			{
//				data = *std::static_pointer_cast<T>(it->second);
//				return true;
//			}
//			return false;
//		}
//
//		template<typename T> bool ChangeData(const std::string& key, T data)
//		{
//			auto it = m_Data.find(key);
//			if (it != m_Data.end())
//			{
//				*std::static_pointer_cast<T>(it->second) = data;
//				return true;
//			}
//			return false;
//		}
//
//	private:
//		std::unordered_map<std::string, std::unique_ptr<void>> m_Data;
//	};
//
//	class SMState
//	{
//	public:
//		SMState(){}
//		virtual ~SMState() = default;
//
//		virtual void OnEnter(SMData* const pData) = 0;
//		virtual void OnExit(SMData* const pData) = 0;
//		virtual void Update(SMData* const pData) = 0;
//	};
//
//	class SMCondition
//	{
//	public:
//		SMCondition() = default;
//		virtual ~SMCondition() = default;
//		virtual bool Evaluate(SMData* const pData) const = 0;
//	};
//
//	class StateMachine final
//	{
//	public:
//		StateMachine(SMState* const startState, std::unique_ptr<SMData> pData)
//			: m_pCurrentState(nullptr), m_pBlackboard(std::move(pData))
//		{
//			ChangeState(startState);
//		}
//
//		~StateMachine();
//
//		void AddTransition(SMState* const startState, SMState* const toState, SMCondition* const transition)
//		{
//			auto it = m_Transitions.find(startState);
//			if (it == m_Transitions.end())
//			{
//				m_Transitions[startState] = Transitions{};
//			}
//
//			m_Transitions[startState].emplace_back(std::make_pair( transition, toState));
//		}
//
//		void Update()
//		{
//			if (m_Transitions.find(m_pCurrentState) != m_Transitions.end())
//			{
//				auto it = m_Transitions.find(m_pCurrentState);
//				Transitions& transitions = it->second;
//				for (auto& transition : transitions)
//				{
//					if (transition.first->Evaluate(m_pBlackboard.get()))
//					{
//						ChangeState(transition.second);
//						break;
//					}
//				}
//			}
//
//			if (m_pCurrentState != nullptr)
//				m_pCurrentState->Update(m_pBlackboard.get());
//		}
//
//		void ChangeState(SMState* const newState)
//		{
//			if (m_pCurrentState)
//			{
//				m_pCurrentState->OnExit(m_pBlackboard.get());
//			}
//			m_pCurrentState = newState;
//			m_pCurrentState->OnEnter(m_pBlackboard.get());
//		}
//
//		SMData* GetSMData() const
//		{
//			return m_pBlackboard.get();
//		}
//
//	private:
//		typedef std::pair<SMCondition*, SMState*> TransitionStatePair;
//		typedef std::vector<TransitionStatePair> Transitions;
//
//		std::unordered_map<SMState*, Transitions> m_Transitions;
//		SMState* m_pCurrentState;
//		std::unique_ptr<SMData> m_pBlackboard;
//	};
//
//}