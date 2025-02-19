#pragma once
#include <string>
#include <functional>
#include <filesystem>

namespace dae
{
	class Minigin final
	{
		bool m_quit{};

		// CONFIG SETTINGS
		float m_FixedTimeStep{ 0.02f };
		int m_MsPerFrame{ 16 };
	public:
		explicit Minigin(const std::filesystem::path& dataPath);
		~Minigin();
		void Run(const std::function<void()>& load);
		void RunOneFrame();

		Minigin(const Minigin& other) = delete;
		Minigin(Minigin&& other) = delete;
		Minigin& operator=(const Minigin& other) = delete;
		Minigin& operator=(Minigin&& other) = delete;
	};
}