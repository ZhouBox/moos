#ifndef MOOS_OBJECT_H
#define MOOS_OBJECT_H



#include "moos_defines.h"
#include "moos_looper.h"


DEFINE_NAMESPACE_MOOS_BEGIN



class MoosObject
{
public:
    MoosObject()
        : m_looper(MoosLooper::currentLooper())
    {

    }


    virtual ~MoosObject() {}

    MoosLooper* eventLooper()
    {
        return m_looper;
    }


private:
    MoosLooper* m_looper;
};




DEFINE_NAMESPACE_MOOS_END

#endif // MOOS_OBJECT_H
