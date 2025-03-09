#include "XInputGamepad.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <Xinput.h>

class XInputGamepad::XInputGamepadImpl
{
	unsigned int m_ButtonsPressedThisFrame{};
	unsigned int m_ButtonsReleasedThisFrame{};
	XINPUT_STATE m_CurrentState{};
	int	m_ControllerIndex{};

public:
	XInputGamepadImpl(int controllerIndex);

	void ProcessInput();
	bool IsDownThisFrame(unsigned int button) const;
	bool IsUpThisFrame(unsigned int button) const;
	bool IsPressed(unsigned int button) const;
};

XInputGamepad::XInputGamepadImpl::XInputGamepadImpl(int controllerIndex)
	: m_ControllerIndex(controllerIndex)
{
}

void XInputGamepad::XInputGamepadImpl::ProcessInput()
{
	XINPUT_STATE previousState{m_CurrentState};
	CopyMemory(&previousState, &m_CurrentState, sizeof(XINPUT_STATE));
	ZeroMemory(&m_CurrentState, sizeof(XINPUT_STATE));
	XInputGetState(m_ControllerIndex, &m_CurrentState);

	auto buttonChanges = m_CurrentState.Gamepad.wButtons ^ previousState.Gamepad.wButtons;
	m_ButtonsPressedThisFrame = buttonChanges & m_CurrentState.Gamepad.wButtons;
	m_ButtonsReleasedThisFrame = buttonChanges & (~m_CurrentState.Gamepad.wButtons);
}

bool XInputGamepad::XInputGamepadImpl::IsDownThisFrame(unsigned int button) const
{
	return m_ButtonsPressedThisFrame & button;
}

bool XInputGamepad::XInputGamepadImpl::IsUpThisFrame(unsigned int button) const
{
	return m_ButtonsReleasedThisFrame & button;
}

bool XInputGamepad::XInputGamepadImpl::IsPressed(unsigned int button) const
{
	return m_CurrentState.Gamepad.wButtons & button;
}

XInputGamepad::XInputGamepad(int controllerIndex)
{
	m_pImpl = new XInputGamepadImpl(controllerIndex);
}

XInputGamepad::~XInputGamepad()
{
	delete m_pImpl;
}

void XInputGamepad::ProcessInput()
{
	m_pImpl->ProcessInput();
}

bool XInputGamepad::IsDownThisFrame(unsigned int button) const
{
	return m_pImpl->IsDownThisFrame(button);
}

bool XInputGamepad::IsUpThisFrame(unsigned int button) const
{
	return m_pImpl->IsUpThisFrame(button);
}

bool XInputGamepad::IsPressed(unsigned int button) const
{
	return m_pImpl->IsPressed(button);
}
