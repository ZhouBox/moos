#include "moos_eventhub.h"

#include <sys/epoll.h>
#include <linux/input.h>
#include <sys/ioctl.h>
#include <sys/inotify.h>


DEFINE_NAMESPACE_MOOS_BEGIN





MoosEventHub::MoosEventHub()
{

}

MoosEventHub::~MoosEventHub()
{

}

size_t MoosEventHub::getEvents(MoosRawEvent *rawEvents_, size_t eventsSize_, int32_t timeOut_)
{
    MOOS_UNUSE(rawEvents_);
    MOOS_UNUSE(eventsSize_);
    MOOS_UNUSE(timeOut_);
    return 0;
}





DEFINE_NAMESPACE_MOOS_END
