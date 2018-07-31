#include <vector>
#pragma warning(push, 0)  
#include "libusb.h"
#pragma warning(pop)
#include <QString>
#include "serialio.h"
#include "error.h"

// ===== SerialInterface =====

SerialInterface::~SerialInterface()
{
}

// ===== USBInterface =====

USBInterface::USBInterface()
{
	// libusb_context only created by first USBInterface
	if (m_context == nullptr)
	{
		auto err = libusb_init(&m_context);
		// throws exceptions
		checkErrorDefault((libusb_error)err);
	}
	// libusb_context deleted when there are no more USBInterfaces
	// need to count the number of USBInterface objects
	++m_context_ref_count;
}

USBInterface::~USBInterface()
{
	close();

	if (m_context != nullptr)
	{
		--m_context_ref_count;
		if (m_context_ref_count == 0)
		{
			libusb_exit(m_context);
			m_context = nullptr;
		}
	}
}

void USBInterface::open(std::vector<uint32_t> properties)
{
	if (properties.size() != size_t(4))
		throw DETAILEDEXCEPTION("'USBInterface::open' requires 4 properties of the form: {PID,VID,Interface,Endpoint}");
	
	close();
	m_pid = uint16_t(properties[0]);
	m_vid = uint16_t(properties[1]);
	m_interface = properties[2];
	m_endpoint = uint8_t(properties[3]);

	// get devices list
	// search for PID + VID
	// get device handle
	// claim interface
	// free device list

	/*
	libusb_device** devices;

	auto count = libusb_get_device_list(nullptr, &devices);
	if (count < (ssize_t)0)
		throw DETAILEDEXCEPTION("Unable to get USB device list");

	for (ssize_t i=0; i<count; ++i)
	{
		libusb_device* device = devices[i];
		// do something with device
	}

	libusb_free_device_list(devices, 1);
	devices = nullptr;


	libusb_device_descriptor desc;
	int r = libusb_get_device_descriptor(dev, &desc);
	
	//int speed = libusb_get_device_speed(device);
	*/

	//TODO: do this the proper way
	libusb_device_handle* device;
	device = libusb_open_device_with_vid_pid(m_context, m_vid, m_pid);

	if (device == nullptr)
		throw DETAILEDEXCEPTION(QString("Failed to open USB device (VendorID: %1 , ProductID: %2)").arg(m_vid).arg(m_pid));

	auto err = libusb_claim_interface(m_device, (int)m_interface);

	if (err < 0)
	{
		checkError((libusb_error)err,LIBUSB_ERROR_NOT_FOUND,"requested interface does not exist");
		checkError((libusb_error)err,LIBUSB_ERROR_BUSY,"another program or driver has claimed the interface");
		checkError((libusb_error)err,LIBUSB_ERROR_NO_DEVICE,"the device has been disconnected");
		checkErrorDefault((libusb_error)err);
	}
}

void USBInterface::close()
{
	if (m_device)
	{
		// unclaim interface
		libusb_release_interface(m_device, m_interface);
		// TODO: errors
		// release handle
		libusb_close(m_device);
		// TODO: errors
		m_device = nullptr;
	}
}

void USBInterface::receivePacket(std::vector<uint8_t>& data_in)
{
	int actual_length = -1;
	auto err = libusb_interrupt_transfer(m_device, (m_endpoint | LIBUSB_ENDPOINT_IN), data_in.data(), data_in.size(), &actual_length, m_timeout);

	// Todo: proper error handling
	if (err == LIBUSB_ERROR_TIMEOUT)
		throw DETAILEDEXCEPTION(QString("USB interrupt poll timeout (period = %1)").arg(m_timeout));
	else if (err < 0)
		throw DETAILEDEXCEPTION(QString("read unsuccessful. Error code: %1 ").arg(err));

	if (actual_length != data_in.size())
		DEBUG_PRINTLNY(QString("Number of read bytes mismatch... required: %1 actual: %2").arg(data_in.size()).arg(actual_length), MSG_TYPE::WARNING);
}

void USBInterface::sendPacket(const std::vector<uint8_t>& data_out)
{
	int actual_length = -1;
	int err = libusb_interrupt_transfer(m_device, (m_endpoint | LIBUSB_ENDPOINT_OUT), const_cast<uint8_t*>(data_out.data()), data_out.size(), &actual_length, m_timeout);

	// Todo: proper error handling
	if (err == LIBUSB_ERROR_TIMEOUT)
		throw DETAILEDEXCEPTION(QString("USB interrupt send timeout (period = %1)").arg(m_timeout));
	else if (err < 0)
		throw DETAILEDEXCEPTION(QString("write unsuccessful (error code: %1)").arg(err));

	if (actual_length != data_out.size())
		DEBUG_PRINTLNY(QString("Number of read bytes mismatch... required: %1 actual: %2").arg(data_out.size()).arg(actual_length), MSG_TYPE::WARNING);
}

void USBInterface::checkErrorDefault(libusb_error err)
{
	if (err == (int)libusb_error::LIBUSB_SUCCESS) return;
	QString msg = QString("ERROR: ") + libusb_error_name(err) + libusb_strerror(libusb_error(err));
	throw DETAILEDEXCEPTION(msg);
}

void USBInterface::checkError(libusb_error err, libusb_error check, QString msg)
{
	if (err == check)
		throw DETAILEDEXCEPTION(QString("%1: %2").arg(libusb_error_name(err)).arg(msg));
}

libusb_context* USBInterface::m_context{nullptr};
uint32_t USBInterface::m_context_ref_count{0};

// ===== COMInterface =====

void COMInterface::open(std::vector<uint32_t> properties)
{
}

void COMInterface::close()
{
}

void COMInterface::receivePacket(std::vector<uint8_t>& data_out)
{
}

void COMInterface::sendPacket(const std::vector<uint8_t>& data_in)
{
}
