#ifndef DEFINES_H
#define DEFINES_H

#include <chrono>

#define DEFINE_NAMESPACE_ZZ_BEGIN namespace zz {

#define DEFINE_NAMESPACE_ZZ_END }

#define UN_USE(X) (void(X))



DEFINE_NAMESPACE_ZZ_BEGIN


const int THREAD_TICK = 30;


typedef std::chrono::milliseconds ms;

enum
{
    TASK_COMMON = 1 << 0,
    TASK_DELAY = 1 << 1,
};

DEFINE_NAMESPACE_ZZ_END




#endif // DEFINES_H
