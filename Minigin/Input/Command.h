#pragma once
#include <unordered_map>

class Command
{
public:
	virtual ~Command() = default;
	virtual void Execute() = 0;
};
