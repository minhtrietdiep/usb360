#include <cstdio>
#include "controllerinput.hxx"

ControllerInput::ControllerInput(libusb_device_handle *h, unsigned char endpointIn) {
	handle = h;
	epIn = endpointIn;
}

bool ControllerInput::Update() {
	lastFunc = "UpdateFunc";
	int transferred = 0;
	libusb_interrupt_transfer(handle, epIn, input, sizeof input, &transferred, 0);
	if (transferred != sizeof(input)) {
		return false;
		//perror("Error while sending ");
	}
	return true;
}

bool ControllerInput::GetButton(ButtonType button) {
	lastFunc = "GetButton";
	if (!input) {
		return false;
	}
	if (button == ButtonXbox) {
		return ((input[3] & 4) == 4);
	}
	if (button == ButtonA) {
		return ((input[3] & 16) == 16);
	}
	if (button == ButtonB) {
		return ((input[3] & 32) == 32);
	}
	if (button == ButtonX) {
		return ((input[3] & 64) == 64);
	}
	if (button == ButtonY) {
		return ((input[3] & 128) == 128);
	}
	if (button == ButtonStart) {
		return ((input[2] & 16) == 16);
	}
	if (button == ButtonBack) {
		return ((input[2] & 32) == 32);
	}
	if (button == DpadUp) {
		return ((input[2] & 1) == 1);
	}
	if (button == DpadDown) {
		return ((input[2] & 2) == 2);
	}
	if (button == DpadLeft) {
		return ((input[2] & 4) == 4);
	}
	if (button == DpadRight) {
		return ((input[2] & 8) == 8);
	}
	if (button == ButtonL) {
		return ((input[2] & 64) == 64);
	}
	if (button == ButtonR) {
		return ((input[2] & 128) == 128);
	}
	if (button == ButtonLB) {
		return ((input[3] & 1) == 1);
	}
	if (button == ButtonRB) {
		return ((input[3] & 2) == 2);
	}
	return false;
}

int ControllerInput::GetTrigger(ButtonType button) {
	lastFunc = "getTrigger";
	if (!input) {
		return 0;
	}
	if (button == TriggerL) {
		return input[4];
	}
	if (button == TriggerR) {
		return input[5];
	}
	return 0;
}

int16_t ControllerInput::GetJoystick(ButtonType button) {
	lastFunc = "getjoystick";
	if (!input) {
		return 0;
	}
	if (button == JoystickLX) {
		return ((input[7] << 8) + input[6]);
	}
	if (button == JoystickLY) {
		return ((input[9] << 8) + input[6]);
	}
	if (button == JoystickRX) {
		return ((input[0x0b] << 8) + input[0x0a]);
	}
	if (button == JoystickRY) {
		return ((input[0x0d] << 8) + input[0x0c]);
	}
	return 0;
}
