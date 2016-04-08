#ifndef MOOS_OBJECT_H
#define MOOS_OBJECT_H

#include "moos_defines.h"
#include "moos_looper.h"

DEFINE_NAMESPACE_ZZ_BEGIN


class MoosObject
{
public:
    virtual ~MoosObject() {}

    Looper* eventLooper()
    {
        return Looper::currentLooper();
    }
};


DEFINE_NAMESPACE_ZZ_END

#endif // MOOS_OBJECT_H
