#ifndef MOOS_EVENTHUB_H
#define MOOS_EVENTHUB_H

#include <chrono>
#include <vector>
#include <string>

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
public:
    MoosDevice(std::string&& deviceName_, int fileId_);

    ~MoosDevice();

    const std::string& name() const
    {
        return m_deviceName;
    }

    int id() const
    {
        return m_fileId;
    }

    void close();
private:
    std::string m_deviceName;
    int m_fileId;

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


private:
    int m_epollId;
    int m_notifyId;

    static const int s_epoll_notify_id = 0x0506;

    static const int s_epoll_max_events = 20;

    epoll_event m_epollEvents[s_epoll_max_events];




};




DEFINE_NAMESPACE_MOOS_END

#endif // MOOS_EVENTHUB_H
