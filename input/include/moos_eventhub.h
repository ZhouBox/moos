#ifndef MOOS_EVENTHUB_H
#define MOOS_EVENTHUB_H

#include <chrono>
#include <vector>
#include <string>
#include <list>

#include <sys/epoll.h>



#include "moos_defines.h"


DEFINE_NAMESPACE_MOOS_BEGIN

struct MoosRawEvent
{
    std::chrono::system_clock::time_point when;
    int32_t deviceId;
    int32_t type;
    int32_t code;
    int32_t value;
};

enum
{
    MOOSINPUT_DEVICE_CLASS_KEYBOARD = 1 << 0,
    MOOSINPUT_DEVICE_CLASS_MOUSE = 1 << 1
};

class MoosDevice
{
    friend class MoosEventHub;
public:
    MoosDevice();


    ~MoosDevice();

    const std::string& name() const
    {
        return m_info.m_deviceName;
    }

    int id() const
    {
        return m_fileId;
    }








    void close();
    int deviceId() const
    {
        return m_deviceId;
    }

    void setDeviceId(int deviceId_)
    {
        m_deviceId = deviceId_;
    }

    struct MoosDeviceInfo
    {
        std::string m_deviceName;
        std::string m_uniqueId;
        uint16_t m_bus;
        uint16_t m_vendor;
        uint16_t m_product;
        uint16_t m_version;
        int m_driverVersion;
    };


    MOOS_NO_CPOY_ASSIGN(MoosDevice);

private:
    std::string m_devicePath;
    int m_fileId;
    static int s_deviceId;
    int m_deviceId;
    MoosDeviceInfo m_info;

};

class MoosEventHub
{
public:


    MoosEventHub();
    ~MoosEventHub();

    enum
    {
        DEVICE_ADD = 1 << 0,
        DEVICE_REMOVE = 1 << 1,

    };

    size_t getEvents(MoosRawEvent* rawEvents_, size_t eventsSize_, int32_t timeOut_ = -1);

    void scanDevices();

private:
    void onInotify();

    void openDevice(const char* devicePath_);
    void removeDevice(const char* devicePath_);

private:
    int m_epollId;
    int m_notifyId;

    static const int s_epoll_notify_id = 0x0506;

    static const int s_epoll_max_events = 20;

    epoll_event m_epollEvents[s_epoll_max_events];

    std::list<MoosDevice*> m_devices;




};




DEFINE_NAMESPACE_MOOS_END

#endif // MOOS_EVENTHUB_H
