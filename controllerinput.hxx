#ifndef CONTROLLERINPUT_H
#define CONTROLLERINPUT_H
#include <string>
#include <libusb-1.0/libusb.h>

extern std::string lastFunc;

enum ButtonType {
	ButtonXbox,
	ButtonA,
	ButtonB,
	ButtonX,
	ButtonY,
	ButtonStart,
	ButtonBack,
	DpadUp,
	DpadDown,
	DpadLeft,
	DpadRight,
	ButtonL,
	ButtonR,
	ButtonLB,
	ButtonRB,
	TriggerL,
	TriggerR,
	JoystickLX,
	JoystickLY,
	JoystickRX,
	JoystickRY
};

class ControllerInput {
	public:
		ControllerInput(libusb_device_handle *h, unsigned char endpointIn);
		bool Update();
		bool GetButton(ButtonType button);
		int GetTrigger(ButtonType button);
		int16_t GetJoystick(ButtonType button);
		
	private:
		libusb_device_handle *handle;
		unsigned char epIn;
		unsigned char input[20];
};

#endif
