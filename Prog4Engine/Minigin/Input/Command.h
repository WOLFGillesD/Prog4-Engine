#pragma once
#include <unordered_map>

#include "GameObject.h"
#include "vec2.hpp"

namespace dae
{
	class Command
	{
	public:
		virtual ~Command() = default;
		virtual void Execute() = 0;
	};
}
