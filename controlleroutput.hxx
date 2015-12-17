#ifndef CONTROLLEROUTPUT_H
#define CONTROLLEROUTPUT_H

#include <libusb-1.0/libusb.h>
#include <vector>
#include <string>

extern std::string lastFunc;


class ControllerOutput {
	public:
		ControllerOutput(libusb_device_handle *h, unsigned char endpointOut);
		void SetRumble(const std::vector<unsigned char>& rumblePattern);
		void SetLeds(unsigned char ledPattern);
	
	private:
		libusb_device_handle *handle;
		unsigned char epOut;
		void sendData(const std::vector<unsigned char>& data);
};

#endif
