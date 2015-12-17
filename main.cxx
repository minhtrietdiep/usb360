#include "controllerinput.hxx"
#include "controlleroutput.hxx"
#include <iostream>
#include <iomanip>
#include <cstdio>
#include <libusb-1.0/libusb.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <string>

#define OFF 0x00
#define ALL_BLINKING 0x01
#define FLASH_1 0x02
#define FLASH_2 0x03
#define FLASH_3 0x04
#define FLASH_4 0x05
#define ON_1 0x06
#define ON_2 0x07
#define ON_3 0x08
#define ON_4 0x09
#define ROTATING 0x0a
#define BLINKING 0x0b
#define SLOW_BLINKING 0x0c
#define ALTERNATING 0x0d

std::string lastFunc = "";

static void handler(int sig, siginfo_t *si, void *unused)
{
    //printf("Got SIGSEGV at address: 0x%lx\n",(long) si->si_addr);
    //printf("Implements the handler only\n");
    //std::cout << lastFunc << std::endl;
    //flag=1;
    exit(0);
}


int main() {
	struct sigaction sa;

    sa.sa_flags = SA_SIGINFO;
    sigemptyset(&sa.sa_mask);
    sa.sa_sigaction = handler;
    if (sigaction(SIGSEGV, &sa, NULL) == -1)
        std::cout << "Doennie" << std::endl;
        //handle_error("sigaction");

	
	
	std::vector<unsigned char> MAXRUMBLE({ 0x00, 0x08, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00 });
	std::vector<unsigned char> NORUMBLE({ 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 });
	
	// These are the Xbox 360
	int vid = 0x045e;
	int pid = 0x028e;

	unsigned char endpointIN = 0x81;
	unsigned char endpointOUT = 0x01;

	int error = 0;

	libusb_context *context = NULL;
	libusb_device_handle *h = NULL;
	error = libusb_init(&context);

	if (error) {
		std::cout << "Failed to initialize libusb: "
			<< libusb_error_name(error) << std::endl;
		return 1;
	}

	h = libusb_open_device_with_vid_pid(NULL, vid, pid);
	if (!h) {
		std::cout << "Failed to open device" << std::endl;
		return 1;
	}
	std::cout << "Opened Xbox 360 controller." << std::endl;

	ControllerInput* controls = new ControllerInput(h, endpointIN);
	ControllerOutput* output = new ControllerOutput(h, endpointOUT);
	//std::cout << "Made things" << std::endl;
	
	output->SetLeds(ROTATING);

	bool monitor = true;
	std::cout << "Monitoring" << std::endl;
	while (monitor) {
		while (!controls->Update()) {
		
		}
		std::cout << "\033[2J\033[1;1H";
		
		std::cout << "LTrig: " << controls->GetTrigger(TriggerL) << "     RTrig: " << controls->GetTrigger(TriggerR) << std::endl;
		std::cout << "LB: " << controls->GetButton(ButtonLB) << "        RB: " << controls->GetButton(ButtonRB) << std::endl;
		std::cout << "LX: " << controls->GetJoystick(JoystickLX) << "     RX: " << controls->GetJoystick(JoystickRX) << std::endl;
		std::cout << "LY: " << controls->GetJoystick(JoystickLY) << "     RY: " << controls->GetJoystick(JoystickRY) << std::endl;
		std::cout << "DU: " << controls->GetButton(DpadUp) << "        Y: " << controls->GetButton(ButtonY) << std::endl;
		std::cout << "DD: " << controls->GetButton(DpadDown) << "        X: " << controls->GetButton(ButtonX) << std::endl;
		std::cout << "DL: " << controls->GetButton(DpadLeft) << "        B: " << controls->GetButton(ButtonB) << std::endl;
		std::cout << "DR: " << controls->GetButton(DpadRight) << "        A: " << controls->GetButton(ButtonA) << std::endl;
		std::cout << "Back: " << controls->GetButton(ButtonBack) << "     Start: " << controls->GetButton(ButtonStart) << std::endl;
		std::cout << "LJoy: " << controls->GetButton(ButtonL) << "     RJoy: " << controls->GetButton(ButtonR) << std::endl;

		if (controls->GetButton(ButtonA)) {
			output->SetRumble(MAXRUMBLE);
		}
		else {
			output->SetRumble(NORUMBLE);
		}
		
		if (controls->GetButton(DpadUp)) {
			output->SetLeds(ON_1);
		}
		
		if (controls->GetButton(DpadDown)) {
			output->SetLeds(ON_2);
		}
		
		if (controls->GetButton(DpadLeft)) {
			output->SetLeds(ON_3);
		}
		
		if (controls->GetButton(DpadRight)) {
			output->SetLeds(ON_4);
		}
		
		if (controls->GetButton(ButtonXbox)) {
			monitor = false;
		}
		//std::cout << "NotDeadYet" << std::endl;
	}
	
	output->SetLeds(OFF);
	
	usleep(100000);
	output->SetRumble(NORUMBLE);
	
	usleep(100000);
	
	libusb_close(h);
	libusb_exit(context);
	std::cout << "Closing..." << std::endl;
	return 0;
}
