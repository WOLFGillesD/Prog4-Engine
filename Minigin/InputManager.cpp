#include <SDL.h>
#include "InputManager.h"

#include <iostream>

#include "Renderer.h"
#include "backends/imgui_impl_sdl2.h"

std::unordered_map<unsigned int, std::vector<dae::InputCommand>> dae::InputMapping::GetCommands()
{
	return m_Commands;
}

void dae::InputMapping::AddCommand(unsigned int button, InputCommand command)
{
	if (m_Commands.contains(button))
	{
		m_Commands[button].push_back(command);
	}
	else
	{
		std::vector<InputCommand> ic{};
		ic.push_back(command);
		std::pair pair{ button, ic };
		m_Commands.insert(pair);
	}
}

void dae::InputMapping::ClearCommandsForButton(unsigned int button)
{
	m_Commands.erase(button);
}

bool dae::InputManager::ProcessInput()
{
	SDL_Event e;

	m_Controller->ProcessInput();

	if (m_ControllerMapping.get() != nullptr)
	{
		for (const auto& map : m_ControllerMapping->GetCommands())
		{
			auto button = map.first;
			auto inputCommands = map.second;

			for (const auto& inputCommand : inputCommands)
			{
				switch (inputCommand.inputState)
				{
				case InputState::IsPressed:
					if (m_Controller->IsPressed(button))
						inputCommand.command->Execute();
					break;
				case InputState::IsUp:
					if (m_Controller->IsUpThisFrame(button))
						inputCommand.command->Execute();
					break;
				case InputState::IsDown:
					if (m_Controller->IsDownThisFrame(button))
						inputCommand.command->Execute();
					break;
				}
			}
		}
	}

	while (SDL_PollEvent(&e))
	{
		ImGui_ImplSDL2_ProcessEvent(&e);

		if (m_ControllerMapping.get() != nullptr)
		{
			if (e.type == SDL_QUIT) { return false; }
			if (e.type == SDL_KEYDOWN)
			{
				auto keyboard = m_KeyBoardMapping->GetCommands();
				auto button = e.key.keysym.sym;
				if (keyboard.contains(button))
				{
					auto inputCommands = keyboard.find(button)->second;
					for (const auto& inputCommand : inputCommands)
					{
						if (inputCommand.inputState == InputState::IsPressed)
						{
							inputCommand.command->Execute();
						}

						if (e.key.repeat == 0 && inputCommand.inputState == InputState::IsDown)
						{
							inputCommand.command->Execute();
						}
					}
				}
			}
			if (e.type == SDL_KEYUP)
			{
				if (e.key.repeat == 0)
				{
					auto keyboard = m_KeyBoardMapping->GetCommands();
					auto button = e.key.keysym.sym;
					if (keyboard.contains(button))
					{
						auto inputCommands = keyboard.find(button)->second;
						for (const auto& inputCommand : inputCommands)
						{
							if (inputCommand.inputState == InputState::IsUp)
							{
								inputCommand.command->Execute();
							}
						}
					}
				}
			}
			if (e.type == SDL_MOUSEBUTTONDOWN)
			{

			}
			if (e.type == SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_CLOSE and e.window.windowID ==
				SDL_GetWindowID(Renderer::GetInstance().GetSDLWindow()))
			{
				return false;
			}
		}
	}
	return true;
}

void dae::InputManager::SetControllerInputMapping(std::unique_ptr<InputMapping> inputMapping)
{
	m_ControllerMapping.release();
	m_ControllerMapping = std::move(inputMapping);
}

void dae::InputManager::SetKeyboardInputMapping(std::unique_ptr<InputMapping> inputMapping)
{
	m_KeyBoardMapping.release();
	m_KeyBoardMapping = std::move(inputMapping);
}

dae::InputMapping* dae::InputManager::GetControllerInputMapping()
{
	return m_ControllerMapping.get();
}

dae::InputMapping* dae::InputManager::GetKeyboardInputMapping()
{
	return m_KeyBoardMapping.get();
}
