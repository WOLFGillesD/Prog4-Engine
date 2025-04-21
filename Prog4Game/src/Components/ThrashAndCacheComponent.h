#pragma once
#include <vector>

#include "Component.h"

namespace dae
{

	class ThrashAndCacheComponent : public Component
	{
	public:
		ThrashAndCacheComponent(GameObject& go);
		~ThrashAndCacheComponent() override = default;

		void Update() override;
		void ImGuiUpdate() override;

		ThrashAndCacheComponent(const ThrashAndCacheComponent& other) = delete;
		ThrashAndCacheComponent(ThrashAndCacheComponent&& other) noexcept = delete;
		ThrashAndCacheComponent& operator=(const ThrashAndCacheComponent& other) = delete;
		ThrashAndCacheComponent& operator=(ThrashAndCacheComponent&& other) noexcept = delete;

	private:

		const int m_TESTSIZE = 10'000'000;
		int m_NrOfSamples = 5;

		std::vector<long long> m_Ex1_x{};
		std::vector<long long> m_Ex2_go_x{};
		std::vector<long long> m_Ex2_goa_x{};

		std::vector<long long> m_Ex1_y{};
		std::vector<long long> m_Ex2_go_y{};
		std::vector<long long> m_Ex2_goa_y{};


		void Exercise1();
		void Exercise2_Go();
		void Exercise2_Goa();

		void SaveToCSV();
	};
}
