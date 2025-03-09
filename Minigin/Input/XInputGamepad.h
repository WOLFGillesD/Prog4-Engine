#pragma once
#include "Gamepad.h"

class XInputGamepad : public Gamepad
{
	class XInputGamepadImpl;
	XInputGamepadImpl* m_pImpl;
public:
	XInputGamepad(int controllerIndex);
	~XInputGamepad() override;

	void ProcessInput() override;
	bool IsDownThisFrame(unsigned int button) const override;
	bool IsUpThisFrame(unsigned int button) const override;
	bool IsPressed(unsigned int button) const override;
};

