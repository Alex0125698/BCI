#pragma once

#include "resources.h"

struct libusb_context;

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
    void open(std::vector<uint32_t> properties) = 0;
    void close() = 0;
    void receivePacket(std::vector<uint8_t>& data_in) = 0;
    void sendPacket(const std::vector<uint8_t>& data_out) = 0;

public: // USBInterface functions
    USBInterface();
    ~USBInterface();
    void setEndpoint(uint32_t endpoint);
    std::string getDeviceSpeed();
    void defaultError(int err);

public: // static functions

private: // variables

private: // static variables
    static libusb_context* context;
    static uint32_t ref_count;
};

// ===== COMMInterface =====

class COMMInterface : public SerialInterface
{
public:
    void open(std::vector<uint32_t> properties) = 0;
    void close() = 0;
    void receivePacket(std::vector<uint8_t>& data_out) = 0;
    void sendPacket(const std::vector<uint8_t>& data_in) = 0;
};
