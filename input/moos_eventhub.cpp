#include "moos_eventhub.h"

#include <sys/epoll.h>
#include <linux/input.h>
#include <sys/ioctl.h>
#include <sys/inotify.h>
#include <unistd.h>
#include <memory.h>
#include <dirent.h>

#include "moos_logger.h"


DEFINE_NAMESPACE_MOOS_BEGIN


const char* MOOS_INPUT_PATH = "/dev/input";
const int MOOS_MAX_DEVICE_NAME = 256;



MoosEventHub::MoosEventHub()
{
    m_epollId = epoll_create(0);
    m_notifyId = inotify_init();
    inotify_add_watch(m_notifyId, MOOS_INPUT_PATH, IN_DELETE | IN_CREATE);
    struct epoll_event _eventItem;
    memset(&_eventItem, 0, sizeof(_eventItem));
    _eventItem.events = EPOLLIN | EPOLLET;
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
    int _eventIndex = 0;
    int _epollResult = epoll_wait(m_epollId, m_epollEvents, s_epoll_max_events, -1);
    if (_epollResult > 0) {
        //
        while (_eventIndex < _epollResult) {
        }
    }


    return 0;
}


void MoosEventHub::onInotify()
{

}

void MoosEventHub::scanDevices()
{
    char _devname[MOOS_MAX_DEVICE_NAME];
    DIR* _dir = NULL;
    dirent* _de = NULL;
    char* _filename;
    _dir = opendir(MOOS_INPUT_PATH);
    if (_dir == NULL) {
        MOOS_DEBUG_LOG("open input devices dir fail!!!!");
    }
    else {
        strcpy(_devname, MOOS_INPUT_PATH);
        _filename = _devname + strlen(_devname);
        *_filename++ = '/';
        while ((_de == readdir(_dir))) {
            if (_de->d_name[0] == '.' &&
                    (_de->d_name[1] == '\0' ||
                     (_de->d_name[1] == '.' && _de->d_name[2] == '\0'))) {
                continue;

            }
            strcpy(_filename, _de->d_name);
            openDevice(_devname);
        }
    }



}

void MoosEventHub::openDevice(const char *deviceName_)
{

}

void MoosEventHub::removeDevice(const char *deviceName_)
{

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
