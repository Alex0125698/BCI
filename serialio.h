#pragma once

#include <vector>
#include <string>
#include <cstdint>
#include <QString>

struct libusb_context;
struct libusb_device_handle;
enum libusb_error;

// ===== SerialInterface =====

class SerialInterface
{
public:
	virtual void open(std::vector<uint32_t> properties) = 0;
	virtual void close() = 0;
	virtual void receivePacket(std::vector<uint8_t>& data_out) = 0;
	virtual void sendPacket(const std::vector<uint8_t>& data_in) = 0;
	virtual ~SerialInterface() = 0;
};

// ===== USBInterface =====

struct USBdevice
{
	uint32_t ProductID;
	uint32_t VendorID;
	uint32_t numInterfaces;
};

class USBInterface : public SerialInterface
{
public: // virtual functions
	void open(std::vector<uint32_t> properties);
	void close();
	void receivePacket(std::vector<uint8_t>& data_in);
	void sendPacket(const std::vector<uint8_t>& data_out);

public: // USBInterface functions
	USBInterface();
	~USBInterface();
	void checkErrorDefault(libusb_error err);
	void checkError(libusb_error err, libusb_error check, QString msg);

public: // static functions

private: // variables
	uint16_t m_pid{ 0 };
	uint16_t m_vid{ 0 };
	uint32_t m_interface{ 0 };
	uint8_t m_endpoint{ 0 };
	libusb_device_handle* m_device{ nullptr };
	const uint32_t m_timeout{ 200 }; // in milliseconds

private: // static variables
	static libusb_context* m_context;
	static uint32_t m_context_ref_count;
};

// ===== COMInterface =====

class COMInterface : public SerialInterface
{
public:
	void open(std::vector<uint32_t> properties);
	void close();
	void receivePacket(std::vector<uint8_t>& data_out);
	void sendPacket(const std::vector<uint8_t>& data_in);
};
