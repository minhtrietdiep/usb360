#include <cstdio>
#include "controlleroutput.hxx"

ControllerOutput::ControllerOutput(libusb_device_handle *h, unsigned char endpointOut) {
	handle = h;
	epOut = endpointOut;
}


void ControllerOutput::sendData(const std::vector<unsigned char>& data) {
		lastFunc = "senddate";

	int transferred = 0;
	libusb_interrupt_transfer(handle, epOut, (unsigned char*)data.data(), data.size(), &transferred, 0);
	if ((size_t)transferred != data.size())
	{
		perror("Error while sending.");
	}
}


void ControllerOutput::SetRumble(const std::vector<unsigned char>& rumblePattern) {

	sendData(rumblePattern);
}

void ControllerOutput::SetLeds(unsigned char ledPattern) {
	std::vector <unsigned char> ledMessage({ 0x01, 0x03, ledPattern });
	sendData(ledMessage);
}
