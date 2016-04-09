#ifndef MOOS_OBJECT_H
#define MOOS_OBJECT_H

#include "moos_defines.h"
#include "moos_looper.h"

DEFINE_NAMESPACE_ZZ_BEGIN


class MoosObject
{
public:
    MoosObject()
        : m_looper(Looper::currentLooper())
    {

    }


    virtual ~MoosObject() {}

    Looper* eventLooper()
    {
        return m_looper;
    }

private:
    Looper* m_looper;
};


DEFINE_NAMESPACE_ZZ_END

#endif // MOOS_OBJECT_H
