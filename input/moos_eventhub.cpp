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
    _eventItem.events = EPOLLIN;
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
    size_t _re = 0;
//    int _eventIndex = 0;
    int _epollResult = epoll_wait(m_epollId, m_epollEvents, s_epoll_max_events, timeOut_);
    do {
        if (_epollResult == 0) {
            // time out
            MOOS_DEBUG_LOG("epoll wait time out!");
            break;

        }

        if (_epollResult < 0) {
            // error
            break;
        }

        for (epoll_event _t : m_epollEvents) {
            auto _device = getDeviceById(_t.data.u32);
            if (!_device) {
                MOOS_DEBUG_LOG("device is NULL!");
                continue;
            }
            input_event _buffer[32];
            int32_t _readSize = read(_device->id(), _buffer, sizeof(input_event) * 32);
            if (_readSize == 0 || (_readSize < 0 && errno == ENODEV)) {
                MOOS_DEBUG_LOG("could not get event, removed?");

            }
            else if (_readSize < 0) {
                if (errno != EAGAIN && errno != EINTR) {
                    MOOS_DEBUG_LOG("could not get event error=", errno);
                }
            }
            else if ((_readSize % sizeof(input_event)) != 0) {
                MOOS_DEBUG_LOG("counld not get event");
            }
            else {
                size_t _eventSize = _readSize / (sizeof(input_event));
                if (eventsSize_ < _eventSize) {
                    MOOS_DEBUG_LOG("some event drop!");
                    _eventSize = eventsSize_;
                }
                for (size_t i = 0; i < _eventSize; ++i) {
                    rawEvents_[i].code = _buffer[i].code;
                    rawEvents_[i].type = _buffer[i].type;
                    rawEvents_[i].value = _buffer[i].value;
                    rawEvents_[i].when = std::chrono::system_clock::now();
                    rawEvents_[i].deviceId = _device->deviceId();

                }
                _re = _eventSize;


            }



        }



    } while(0);



    return _re;
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
        MoosDevice::MoosDeviceInfo _deviceInfo;
        if (ioctl(_fd, EVIOCGNAME(sizeof(buffer) - 1), &buffer) < 1) {

        }
        else {
            buffer[sizeof(buffer) - 1] = '\0';
            _deviceInfo.m_deviceName = buffer;
        }


        for (MoosDevice* _t : m_devices) {
            if (_t->name() == _deviceInfo.m_deviceName) {
                MOOS_DEBUG_LOG("ignore ", devicePath_, _deviceInfo.m_deviceName);
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

        _deviceInfo.m_driverVersion = _driverVarsion;


        struct input_id _inputid;
        if (ioctl(_fd, EVIOCGID, &_inputid)) {
            MOOS_DEBUG_LOG("no get input_id for ", devicePath_, strerror(errno));
            close(_fd);
            return;

        }

        _deviceInfo.m_bus = _inputid.bustype;
        _deviceInfo.m_product = _inputid.product;
        _deviceInfo.m_vendor = _inputid.vendor;
        _deviceInfo.m_version = _inputid.version;

        if (ioctl(_fd, EVIOCGUNIQ(sizeof(buffer) - 1), &buffer) < 1) {

        }
        else {
            buffer[sizeof(buffer) - 1] = '\0';
            _deviceInfo.m_uniqueId = buffer;
        }

        if (fcntl(_fd, F_SETFL, O_NONBLOCK)) {
            MOOS_DEBUG_LOG("set fd no block fail!!");
            close(_fd);
            return;
        }

        MoosDevice *_device = new MoosDevice();
        _device->m_info = _deviceInfo;
        _device->m_devicePath = devicePath_;
        _device->m_fileId = _fd;



        struct epoll_event _event;
        memset(&_event, 0, sizeof(_event));
        _event.events = EPOLLIN;
        _event.data.u32 = _fd;
        if (epoll_ctl(m_epollId, EPOLL_CTL_ADD, _fd, &_event)) {
            MOOS_DEBUG_LOG("not add device to epoll error= ", errno);
            delete _device;
            return;

        }
        MOOS_DEBUG_LOG("add device ", _device->name(), "in epoll");

        m_devices.push_back(_device);

    }

}



void MoosEventHub::removeDevice(const char *devicePath_)
{

}

MoosDevice *MoosEventHub::getDeviceById(uint32_t id_)
{
    for (MoosDevice* _t : m_devices) {
        if (_t->id() == id_) {
            return _t;
        }
    }

    return NULL;
}



int MoosDevice::s_deviceId = 0;


MoosDevice::MoosDevice()
    : m_devicePath()
    , m_fileId(-1)
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
