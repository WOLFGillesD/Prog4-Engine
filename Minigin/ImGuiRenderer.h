#pragma once
#include "Singleton.h"

namespace dae
{
	class ImGuiRenderer final : public Singleton<ImGuiRenderer>
	{
	public:
		void Init();
		void Render();
		void Destroy();
	};
}
