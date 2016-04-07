#ifndef MOOS_SIGNAL_H
#define MOOS_SIGNAL_H

#include <memory>
#include <list>

#include "moos_defines.h"

DEFINE_NAMESPACE_ZZ_BEGIN



template <typename ... Args>
class Slot
{



    struct __Impl_base
    {
        virtual void run(Args& ... args) = 0;
    };

    template <typename _Class, typename _Callable>
    struct __Impl : public __Impl_base
    {
        __Impl(_Class* object_, const _Callable& fun_)
            : m_object(object_)
            , m_Mfun(fun_)
        {

        }

        void run(Args& ... args)
        {
            (m_object->*m_Mfun)(args...);
        }

        _Class* m_object;
        _Callable m_Mfun;
    };

public:
    template <typename _Class, typename _Callable>
    Slot(_Class* object_, const _Callable& fun_, CONNECT_TYPE type_)
        : m_type(type_)
    {
        m_m = std::make_shared<__Impl<_Class, _Callable>>(object_, fun_);
    }


    void run(Args& ... args)
    {
        m_m->run(args...);
    }

    int type() const
    {
        return m_type;
    }




private:

    std::shared_ptr<__Impl_base> m_m;
    CONNECT_TYPE m_type;


};






template <typename ... Args>
class Signal
{
public:
    Signal()
    {

    }

    template <typename _Class, typename _Callable>
    bool connect(_Class* object_, const _Callable& fun_, CONNECT_TYPE type_ = CONNECT_AUTO)
    {
        Slot<Args...>* _t = new Slot<Args...>(object_, fun_, type_);
        m_slots.push_back(_t);
        return true;
    }


    void emit(Args ... args)
    {
        for(Slot<Args...>* t : m_slots)
        {
            t->run(args...);
        }
    }




private:

    std::list<Slot<Args...>*> m_slots;


};











DEFINE_NAMESPACE_ZZ_END


#endif // MOOS_SIGNAL_H
