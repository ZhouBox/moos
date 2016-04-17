#include "moos_eventhub.h"

#include <sys/epoll.h>
#include <linux/input.h>
#include <sys/ioctl.h>
#include <sys/inotify.h>
#include <unistd.h>
#include <memory.h>


DEFINE_NAMESPACE_MOOS_BEGIN


const char* MOOS_INPUT_PATH = "/dev/input";




MoosEventHub::MoosEventHub()
{
    m_epollId = epoll_create(0);
    m_notifyId = inotify_init();
    inotify_add_watch(m_notifyId, MOOS_INPUT_PATH, IN_DELETE | IN_CREATE);
    struct epoll_event _eventItem;
    memset(&_eventItem, 0, sizeof(_eventItem));
    _eventItem.events = EPOLLIN;
    _eventItem.data.u32 = s_epoll_notify_id;
    epoll_ctl(m_epollId, EPOLL_CTL_ADD, m_notifyId, &_eventItem);

}

MoosEventHub::~MoosEventHub()
{
    ::close(m_epollId);
    ::close(m_notifyId);
}

size_t MoosEventHub::getEvents(MoosRawEvent *rawEvents_, size_t eventsSize_, int32_t timeOut_)
{
    MOOS_UNUSE(rawEvents_);
    MOOS_UNUSE(eventsSize_);
    MOOS_UNUSE(timeOut_);
    return 0;
}

MoosDevice::MoosDevice(std::string &&deviceName_, int fileId_)
    : m_deviceName(std::forward<std::string>(deviceName_))
    , m_fileId(fileId_)
{

}

MoosDevice::~MoosDevice()
{

}

void MoosDevice::close()
{

    ::close(m_fileId);
}







DEFINE_NAMESPACE_MOOS_END
