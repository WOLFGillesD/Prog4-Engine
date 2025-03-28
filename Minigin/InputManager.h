#pragma once
#include <memory>

#include "Singleton.h"
#include "Input/Command.h"
#include "Input/Gamepad.h"
#include "Input/XInputGamepad.h"

namespace dae
{
	enum class InputState
	{
		IsPressed,
		IsUp,
		IsDown
	};

	struct InputCommand
	{
		Command* command;
		InputState inputState;
	};

	class InputMapping
	{
		std::unordered_map<unsigned int, std::vector<InputCommand>> m_Commands;
	public:
		std::unordered_map<unsigned int, std::vector<InputCommand>> GetCommands();
		void AddCommand(unsigned int button, InputCommand command);
		void ClearCommandsForButton(unsigned int button);

		~InputMapping();
	};

	class InputManager final : public Singleton<InputManager>
	{
		std::vector<std::unique_ptr<Gamepad>> m_Controllers{};
		std::vector<std::unique_ptr<InputMapping>> m_ControllerMappings;
		std::unique_ptr<InputMapping> m_KeyBoardMapping;
	public:
		bool ProcessInput();

		void SetControllerInputMapping(std::unique_ptr<InputMapping> inputMapping, int controllerIndx);
		void SetKeyboardInputMapping(std::unique_ptr<InputMapping> inputMapping);

		void RegisterGamepad(std::unique_ptr<Gamepad> gp);
		//void UnRegisterGamepad(int index);

		InputMapping* GetControllerInputMapping(int controllerIndx);
		InputMapping* GetKeyboardInputMapping();
	};
}
