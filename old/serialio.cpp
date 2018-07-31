/*#include "libusb.h"
#include "serialio.h"

// ===== SerialInterface =====

SerialInterface::~SerialInterface()
{
}

// ===== USBInterface =====

void USBInterface::open(std::vector<uint32_t> properties)
{
    // get devices list
    // search for PID + VID
    // get device handle
    // claim interface
    // free device list
    libusb_device** devices;

    ssize_t count = libusb_get_device_list(nullptr, &devices);
    if (count < 0)
        throw DETAILEDEXCEPTION("Unable to get USB device list");

    for (auto i=0; i<count; ++i)
    {
        libusb_device* device = devices[i];
    }

    libusb_free_device_list(devices, 1);
    devices = nullptr;


    libusb_device_descriptor desc;
    int r = libusb_get_device_descriptor(dev, &desc);


    libusb_device_handle* device;

    int libusb_get_device_speed(device);

    device = libusb_open_device_with_vid_pid(context, 0,0);

    int err = libusb_claim_interface(device, 0);

    auto checkErr = [&](int err, int check, QString msg[])
    {
        if(err == check)
        {
            QString tmp = libusb_error_name(err);
            throw DETAILEDEXCEPTION("ERROR: "+tmp+" "+msg);
        }
    };

    if (err < 0)
    {
        checkErr(err,LIBUSB_ERROR_NOT_FOUND,"requested interface does not exist");
        checkErr(err,LIBUSB_ERROR_BUSY,"another program or driver has claimed the interface");
        checkErr(err,LIBUSB_ERROR_NO_DEVICE,"the device has been disconnected");
        defaultError(err);
    }

    libusb_release_interface(device, 0);

    libusb_close(device);

}

void USBInterface::close()
{
    // unclaim interface
    // release handle
}

void USBInterface::receivePacket(std::vector<uint8_t>& data_in)
{

}

void USBInterface::sendPacket(const std::vector<uint8_t>& data_out)
{

}

void USBInterface::USBInterface()
{
    // libusb_context created by first USBInterface
    if (context == nullptr)
    {
        if (libusb_init(&context) == LIBUSB_ERROR)
            throw DETAILEDEXCEPTION("Could not create libusb context");
    }
    // libusb_context deleted when there are no more USBInterfaces
    // need to count the number of USBInterface objects
    ++ref_count;
}

void USBInterface::~USBInterface()
{
    if (context != nullptr)
    {
        --ref_count;
        if (ref_count == 0)
        {
            libusb_exit(context);
            context = nullptr;
        }
    }
}

void USBInterface::setEndpoint(uint32_t endpoint)
{





}

std::string USBInterface::getDeviceSpeed()
{
    //

}

void USBInterface::defaultError(int err)
{
    throw DETAILEDEXCEPTION(QString("ERROR: ")+libusb_error_name(err)+libusb_strerror(err));
}

libusb_context* USBInterface::context{nullptr};
uint32_t USBInterface::ref_count{0};

// ===== COMMInterface =====





*/
