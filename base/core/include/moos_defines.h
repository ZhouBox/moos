#ifndef MOOS_DEFINES_H
#define MOOS_DEFINES_H

#include <chrono>

#define DEFINE_NAMESPACE_MOOS_BEGIN namespace Moos {

#define DEFINE_NAMESPACE_MOOS_END }

#define UN_USE(X) (void(X))


#define NO_CPOY_ASSIGN(CLASS) \
    CLASS(const CLASS&); \
    CLASS& operator=(const CLASS&) \




#define HAS_MEMBER(member)\
    template<typename T, typename... Args>struct has_member_##member\
{\
    private:\
    template<typename U> static auto Check(int) -> decltype(std::declval<U>().member(std::declval<Args>()...), std::true_type()); \
    template<typename U> static auto Check(...) -> decltype(std::false_type()); \
    public:\
    static const bool value = std::is_same<decltype(Check<T>(0)), std::true_type>::value; \
    } \



DEFINE_NAMESPACE_MOOS_BEGIN











DEFINE_NAMESPACE_MOOS_END




#endif // MOOS_DEFINES_H
