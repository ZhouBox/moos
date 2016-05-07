#ifndef MOOS_LOGGER_H
#define MOOS_LOGGER_H


#include "moos_defines.h"


#include <iostream>



DEFINE_NAMESPACE_MOOS_BEGIN


template <typename T>
void unpack(T&& t)
{
    std::cout << std::forward<T>(t) << ' ';
}

template <typename ... Args>
void debugLogImpl(Args&& ... args)
{
    int dummy[] = {0 , (unpack(std::forward<Args>(args)), 0)...};
    MOOS_UNUSE(dummy);
    std::cout << '\n';
}


template <typename ... Args>
void debugLog(Args&& ... args)
{
    debugLogImpl(std::forward<Args>(args)...);
}



//template <typename T0>
//void debugLog(const T0& t0)
//{
//    debugLogImpl(t0);
//}

//template <typename T0, typename T1>
//void debugLog(const T0& t0, const T1& t1)
//{
//    debugLogImpl(t0, t1);
//}

//template <typename T0, typename T1, typename T2>
//void debugLog(const T0& t0, const T1& t1, const T2& t2)
//{
//    debugLogImpl(t0, t2, t1);
//}

//template <typename T0, typename T1, typename T2, typename T3>
//void debugLog(const T0& t0, const T1& t1, const T2& t2, const T3& t3)
//{
//    debugLogImpl(t0, t3, t2, t1);
//}

//template <typename T0, typename T1, typename T2, typename T3, typename T4>
//void debugLog(const T0& t0, const T1& t1, const T2& t2, const T3& t3, const T4& t4)
//{
//    debugLogImpl(t0, t4, t3, t2, t1);
//}


//template <typename T0, typename T1, typename T2, typename T3, typename T4,
//            typename T5>
//void debugLog(const T0& t0, const T1& t1, const T2& t2, const T3& t3, const T4& t4,
//            const T5& t5)
//{
//    debugLogImpl(t0, t5, t4, t3, t2, t1);
//}

//template <typename T0, typename T1, typename T2, typename T3, typename T4,
//        typename T5, typename T6>
//void debugLog(const T0& t0, const T1& t1, const T2& t2, const T3& t3, const T4& t4,
//        const T5& t5, const T6& t6)
//{
//    debugLogImpl(t0, t6, t5, t4, t3, t2, t1);
//}

//template <typename T0, typename T1, typename T2, typename T3, typename T4,
//        typename T5, typename T6, typename T7>
//void debugLog(const T0& t0, const T1& t1, const T2& t2, const T3& t3, const T4& t4,
//            const T5& t5, const T6& t6, const T7& t7)
//{
//    debugLogImpl(t0, t7, t6, t5, t4, t3, t2, t1);
//}

//template <typename T0, typename T1, typename T2, typename T3, typename T4,
//        typename T5, typename T6, typename T7, typename T8>
//void debugLog(const T0& t0, const T1& t1, const T2& t2, const T3& t3, const T4& t4,
//            const T5& t5, const T6& t6, const T7& t7, const T8& t8)
//{
//    debugLogImpl(t0, t8, t7, t6, t5, t4, t3, t2, t1);
//}



//template <typename T0, typename T1, typename T2, typename T3, typename T4
//          , typename T5, typename T6, typename T7, typename T8, typename T9>
//void debugLog(const T0& t0, const T1& t1, const T2& t2, const T3& t3, const T4& t4
//              , const T5& t5 ,const T6& t6, const T7& t7, const T8& t8, const T9& t9)
//{
//    debugLogImpl(t0, t9, t8, t7, t6, t5,  t4, t3, t2, t1);
//}





DEFINE_NAMESPACE_MOOS_END


#define MOOS_DEBUG_LOG(...) Moos::debugLog("filename:", __FILE__, " line:", __LINE__, " ", __VA_ARGS__)

#endif // MOOS_LOGGER_H
