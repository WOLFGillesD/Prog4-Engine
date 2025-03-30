#include "DaeTime.h"

float dae::Time::m_Lag{ 0.0f };
float dae::Time::m_DeltaTime{};
float dae::Time::m_FixedTimeStep{};
std::chrono::time_point<std::chrono::steady_clock> dae::Time::m_Last_time{};