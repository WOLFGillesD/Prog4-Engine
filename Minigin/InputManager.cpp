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

	for (const auto & map : m_Mapping->GetCommands())
	{
		auto button = map.first;
		auto inputCommands = map.second;

		for (const auto & inputCommand : inputCommands)
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

	if (m_Controller->IsDownThisFrame(static_cast<unsigned int>(GamePadInput::GAMEPAD_A)))
	{
		std::cout << "Hello" << std::endl;
	}

	while (SDL_PollEvent(&e)) 
	{
		ImGui_ImplSDL2_ProcessEvent(&e);

		if (e.type == SDL_QUIT) { return false;	}
		if (e.type == SDL_KEYDOWN) 
		{
			
		}
		if (e.type == SDL_MOUSEBUTTONDOWN) 
		{
			
		}
		if (e.type == SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_CLOSE and e.window.windowID ==
			SDL_GetWindowID(Renderer::GetInstance().GetSDLWindow())) 
		{
			return false;
		}
		// etc...
	}

	return true;
}

void dae::InputManager::SetInputMapping(std::unique_ptr<InputMapping> inputMapping)
{
	m_Mapping.release();
	m_Mapping = std::move(inputMapping);
}

dae::InputMapping* dae::InputManager::GetInputMapping()
{
	return m_Mapping.get();
}
