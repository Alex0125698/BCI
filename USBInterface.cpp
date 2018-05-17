#include "USBInterface.h"
#include "resources.h"

#include <QDebug>
#include <QVector>
#include <sstream>

#include "DetailedException.h"
#pragma warning(push, 0)        
#include "libusb.h"
#pragma warning(pop)

bool USBInterface::m_initialisd{ false };

unsigned char& RawUSBData::operator[](const size_t index)
{
    return data[index];
}

QString RawUSBData::convert2str()
{
    std::stringstream raw_usb_in;

    for (int i = 0; i < RawUSBData::LENGTH; ++i)
    {
        raw_usb_in << std::setw(2) << std::setfill('0') << std::hex << static_cast<uint>(data[i]);

        if ((i + 1) % 2 == 0)
        {
            if ((i + 1) % 16 == 0)
                raw_usb_in << '\n';
            else if ((i + 1) % 8 == 0)
                raw_usb_in << " | ";
            else
                raw_usb_in << ' ';
        }
    }

    return QString::fromStdString(raw_usb_in.str());
}

USBInterface::USBInterface()
{
	if (!m_initialisd)
	{
		//initialize a libUSB session
		int err = libusb_init(&m_ctx);
		if (err < 0)
            throw DETAILEDEXCEPTION(QString("LibUSB failed to initialise. Error Code: %1").arg(err));
		
		m_initialisd = true;

		//set verbosity level to 3, as suggested in the documentation
		libusb_set_debug(m_ctx, 3);
	}

	//get the list of devices
	m_num_devices = libusb_get_device_list(m_ctx, &m_devs);
	if (m_num_devices < 0)
        throw DETAILEDEXCEPTION(QString("Could not retrieve list of devices. Error code: %1").arg(m_num_devices));
}

USBInterface::~USBInterface()
{
	if (m_usb_device)
	{
		libusb_release_interface(m_usb_device, 0); //TODO interface?
		libusb_close(m_usb_device);
		m_usb_device = nullptr;
	}
	if (m_devs)
	{
		libusb_free_device_list(m_devs, 1); //free the list, unref the devices in it
		m_devs = nullptr;
	}
	if (m_ctx)
	{
		libusb_exit(m_ctx); //close the session
		m_ctx = nullptr;
	}
}

struct usb_device
{
	const uint32_t VendorID;
	const uint32_t ProductID;
	const uint32_t NumConfigurations;
	const uint32_t DeviceClass;

};

void USBInterface::print_device_helper(libusb_device* dev)
{
	libusb_device_descriptor desc;
	int r = libusb_get_device_descriptor(dev, &desc);
	if (r < 0)
	{
		qDebug() << "failed to get device descriptor" << '\n';
		return;
	}

	qDebug() << "Number of possible configurations: " << (int)desc.bNumConfigurations << "  ";
	qDebug() << "Device Class: " << (int)desc.bDeviceClass << "  ";
	qDebug() << "VendorID: " << desc.idVendor << "  ";
	qDebug() << "ProductID: " << desc.idProduct << '\n';

	libusb_config_descriptor *config;
	int err = libusb_get_config_descriptor(dev, 0, &config);
	if (err < 0)
	{
		qDebug() << "Error getting config descriptor" << '\n';
		return;
	}

	qDebug() << "Interfaces: " << (int)config->bNumInterfaces << " ||| ";
	const libusb_interface *inter;
	const libusb_interface_descriptor *interdesc;
	const libusb_endpoint_descriptor *epdesc;

	for (int i = 0; i<(int)config->bNumInterfaces; i++)
	{
		inter = &config->interface[i];
		qDebug() << "Number of alternate settings: " << inter->num_altsetting << " | ";
		for (int j = 0; j<inter->num_altsetting; j++)
		{
			interdesc = &inter->altsetting[j];
			qDebug() << "Interface Number: " << (int)interdesc->bInterfaceNumber << " | ";
			qDebug() << "Number of endpoints: " << (int)interdesc->bNumEndpoints << " | ";
			for (int k = 0; k<(int)interdesc->bNumEndpoints; k++)
			{
				epdesc = &interdesc->endpoint[k];
				qDebug() << "Length of Descriptor: " << (int)epdesc->bLength << " | ";
				qDebug() << "Descriptor Type: " << (int)epdesc->bDescriptorType << " | ";
				qDebug() << "EP Address: " << (int)epdesc->bEndpointAddress << " | ";
				qDebug() << "EP Type: " << (int)epdesc->bmAttributes << " | ";
				qDebug() << "Max Packet size: " << (int)epdesc->wMaxPacketSize << " | ";
				qDebug() << "Polling Interval: " << (int)epdesc->bInterval << " | ";
			}
		}
	}
	qDebug() << "\n\n\n";
	libusb_free_config_descriptor(config);
}

void USBInterface::printConnectedDeviceDetails()
{
	qDebug() << m_num_devices << " Devices in list." << "\n\n";
	for (int i = 0; i < m_num_devices; i++)
	{
		qDebug() << "getting stats for device #" << i + 1 << '\n';
		print_device_helper(m_devs[i]); //print specs of this device
		qDebug() << '\n';
	}
}

void USBInterface::connectToDevice(uint16_t VendorID, uint16_t ProductID, int interface_num)
{
	m_usb_device = libusb_open_device_with_vid_pid(m_ctx, VendorID, ProductID);

	if (m_usb_device == nullptr)
        throw DETAILEDEXCEPTION(QString("Failed to open USB device (vid %1 , pid: %2)").arg(VendorID).arg(ProductID));

    emit sendDebugMessage(QString("Successfully opened device (vid %1 , pid: %2)").arg(VendorID).arg(ProductID), __FILE__, __LINE__, MSG_TYPE::GOOD_NEWS);

	/*
	// only on linux ???
	int result = libusb_kernel_driver_active(m_usb_device, 0);
	if (result == 1)
	{
		qDebug() << "a kernel driver is attached - lets detach it" << endl;
		if (libusb_detach_kernel_driver(joystick, 0) == 0)
			qDebug() << "Kernel Driver Detached!" << endl;
	}
	*/

	int err = libusb_claim_interface(m_usb_device, interface_num);
	if (err < 0)
        throw DETAILEDEXCEPTION(QString("Failed to claim interface: %1").arg(interface_num));
	
    emit sendDebugMessage("Successfully claimed interface 0 !!!", __FILE__, __LINE__, MSG_TYPE::GOOD_NEWS);
}

void USBInterface::pollDataFromDevice(RawUSBData& data, int length, unsigned char endpoint_num)
{
	int actual_length = -1;
	int err = libusb_interrupt_transfer(m_usb_device, (endpoint_num | LIBUSB_ENDPOINT_IN), data.data, length, &actual_length, m_timeout);
	
	if (err == LIBUSB_ERROR_TIMEOUT)
        throw DETAILEDEXCEPTION(QString("USB interrupt poll timeout (period = %1)").arg(m_timeout));
	else if (err < 0)
        throw DETAILEDEXCEPTION(QString("read unsuccessful. Error code: %1 ").arg(err));

	if (actual_length != length)
        emit sendDebugMessage(QString("Number of read bytes mismatch... required: %1 actual: %2").arg(length).arg(actual_length), __FILE__, __LINE__, MSG_TYPE::WARNING);
}

void USBInterface::sendDatatoDevice(const RawUSBData& data, int length, unsigned char endpoint_num)
{
	int actual_length = -1;
	int err = libusb_interrupt_transfer(m_usb_device, (endpoint_num | LIBUSB_ENDPOINT_OUT), const_cast<unsigned char*>(data.data), length, &actual_length, m_timeout);
	
	if (err == LIBUSB_ERROR_TIMEOUT)
        throw DETAILEDEXCEPTION(QString("USB interrupt send timeout (period = %1)").arg(m_timeout));
	else if (err < 0)
        throw DETAILEDEXCEPTION(QString("write unsuccessful (error code: %1)").arg(err));

	// TODO
	//if (actual_length != length)
	//	emit sendDebugMessage(QString("Number of sent bytes mismatch... required: %1 actual: %2").arg(length).arg(actual_length), __LINE__, __FILE__, MSG_TYPE::WARNING);
}
