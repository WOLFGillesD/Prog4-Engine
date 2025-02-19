#pragma once
#include <chrono>

namespace dae
{
	class Time
	{
	public:
		static float m_Lag;
		static float m_DeltaTime;
		static float m_FixedTimeStep;
		static std::chrono::time_point<std::chrono::steady_clock> m_Last_time;
	};
}
