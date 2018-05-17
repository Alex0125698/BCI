/* ****************************************************************

Use this class to communicate with another device via USB

start by calling 'init' to set things up
call 'connectToDevice' with the appropriate PID and VID
use 'pollDataFromDevice' and 'sendDatatoDevice' to receive/send data

**************************************************************** */

#pragma once

#include <memory>
#include <QObject>

// forward declerations
struct libusb_context;
struct libusb_device;
struct libusb_device_handle;

struct RawUSBData
{
    enum { LENGTH = 64 };
    unsigned char data[LENGTH+1]{ 0 };
    unsigned char& operator[] (const size_t index);
    QString convert2str();
};

class USBInterface : public QObject
{
	Q_OBJECT

signals:
    void sendDebugMessage(QString what,QString file,int line,int fatal); // NTC

public:
	USBInterface();
	~USBInterface();
	void connectToDevice(uint16_t VendorID, uint16_t ProductID, int interface_num = 0);
	void pollDataFromDevice(RawUSBData& data, int length, unsigned char endpoint_num = 1);
	void sendDatatoDevice(const RawUSBData& data, int length, unsigned char endpoint_num = 1);
	// TODO modify this for USB dialog
	void printConnectedDeviceDetails();
	
private:
	libusb_context* m_ctx{ nullptr };
	// used to retrieve a list of devices
	libusb_device** m_devs{ nullptr };
	// a handle to a specific device
	libusb_device_handle* m_usb_device{ nullptr };
	// timeout period in milliseconds
    unsigned int m_timeout = 1000;
	//holding number of devices in list
	int m_num_devices = 0; 
	void print_device_helper(libusb_device* dev);
	// disable the copy constructor and assignment operator
	USBInterface(const USBInterface&) = delete;
	USBInterface& operator= (const USBInterface&) = delete;
	// only the first USBInterface object will initialise libusb
	static bool m_initialisd;
};
