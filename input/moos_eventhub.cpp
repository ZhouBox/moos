#include "moos_eventhub.h"

#include <sys/epoll.h>
#include <linux/input.h>
#include <sys/ioctl.h>
#include <sys/inotify.h>
#include <unistd.h>
#include <memory.h>
#include <dirent.h>
#include <fcntl.h>
#include <assert.h>

#include "moos_logger.h"


DEFINE_NAMESPACE_MOOS_BEGIN


const char* MOOS_INPUT_PATH = "/dev/input";
const int MOOS_MAX_DEVICE_NAME = 256;



MoosEventHub::MoosEventHub()
{
    m_epollId = epoll_create(1);
    if (m_epollId < 0) {
        MOOS_DEBUG_LOG("Can not create epoll id ", strerror(errno));
        assert(0);
    }
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
    close(m_epollId);
    close(m_notifyId);
    for (MoosDevice* _t : m_devices) {
        delete _t;
    }
    m_devices.clear();
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
        while ((_de = readdir(_dir))) {
            if (_de->d_name[0] == '.' &&
                    (_de->d_name[1] == '\0' ||
                     (_de->d_name[1] == '.' && _de->d_name[2] == '\0'))) {
                continue;

            }
            strcpy(_filename, _de->d_name);
            openDevice(_devname);
        }
    }

    closedir(_dir);



}

void MoosEventHub::openDevice(const char *devicePath_)
{
    MOOS_DEBUG_LOG("open device ", devicePath_);
    char buffer[80];
    int _fd = open(devicePath_, O_RDWR | O_CLOEXEC);

    if (_fd < 0) {
        MOOS_DEBUG_LOG("open device fail!! : ", strerror(errno));
    }
    else {
        MoosDevice *_device = new MoosDevice();
        if (ioctl(_fd, EVIOCGNAME(sizeof(buffer) - 1), &buffer) < 1) {

        }
        else {
            buffer[sizeof(buffer) - 1] = '\0';
            _device->setName(buffer);
        }


        for (MoosDevice* _t : m_devices) {
            if (_t->m_deviceName == _device->m_deviceName) {
                MOOS_DEBUG_LOG("ignore ", devicePath_, _device->m_deviceName);
                close(_fd);
                return;
            }
        }

        int _driverVarsion;
        if (ioctl(_fd, EVIOCGVERSION, &_driverVarsion)) {
            MOOS_DEBUG_LOG("no get driver version for ", devicePath_, strerror(errno));
            close(_fd);
            return;
        }

        _device->m_driverVersion = _driverVarsion;


        struct input_id _inputid;
        if (ioctl(_fd, EVIOCGID, &_inputid)) {
            MOOS_DEBUG_LOG("no get input_id for ", devicePath_, strerror(errno));
            close(_fd);
            return;

        }

        _device->m_bus = _inputid.bustype;
        _device->m_product = _inputid.product;
        _device->m_vendor = _inputid.vendor;
        _device->m_version = _inputid.version;

        if (ioctl(_fd, EVIOCGUNIQ(sizeof(buffer) - 1), &buffer) < 1) {

        }
        else {
            buffer[sizeof(buffer) - 1] = '\0';
            _device->setUniqueId(buffer);
        }

        if (fcntl(_fd, F_SETFL, O_NONBLOCK)) {
            MOOS_DEBUG_LOG("set fd no block fail!!");
            close(_fd);
            return;
        }

        _device->m_devicePath = devicePath_;
        _device->m_fileId = _fd;

        m_devices.push_back(_device);






    }

}



void MoosEventHub::removeDevice(const char *devicePath_)
{

}



int MoosDevice::s_deviceId = 0;


MoosDevice::MoosDevice()
    : m_deviceName()
    , m_devicePath()
    , m_fileId(-1)
{
    m_deviceId = ++s_deviceId;
}

MoosDevice::MoosDevice(std::string &&deviceName_, int fileId_)
    : m_deviceName(std::forward<std::string>(deviceName_))
    , m_fileId(fileId_)
{
    m_deviceId = ++s_deviceId;
}

MoosDevice::~MoosDevice()
{

}

void MoosDevice::close()
{

    ::close(m_fileId);
}








DEFINE_NAMESPACE_MOOS_END
