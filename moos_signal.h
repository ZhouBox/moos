#ifndef MOOS_SIGNAL_H
#define MOOS_SIGNAL_H

#include <memory>
#include <list>
#include <type_traits>
#include <cassert>

#include "moos_defines.h"
#include "moos_looper.h"

DEFINE_NAMESPACE_ZZ_BEGIN



HAS_MEMBER(eventLooper);

template <typename ... Args>
class Slot
{



    struct __Impl_base
    {
        virtual void run(Args&& ... args_) = 0;
        virtual CommonTask* convertTask(Args&& ... args_) = 0;
    };

    template <typename _Class, typename _Callable>
    struct __Impl : public __Impl_base
    {
        __Impl(_Class* object_, const _Callable& fun_)
            : m_object(object_)
            , m_Mfun(fun_)
        {

        }


        CommonTask* convertTask(Args&& ... args_)
        {
            return new CommonTask(std::bind(m_Mfun, m_object, std::forward<Args>(args_)...));
        }

        void run(Args&& ... args_)
        {
            (m_object->*m_Mfun)(std::forward<Args>(args_)...);
        }

        _Class* m_object;
        _Callable m_Mfun;
    };

public:
    template <typename _Class, typename _Callable>
    Slot(_Class* object_, const _Callable& fun_, CONNECT_TYPE type_)
        : m_type(type_)
    {
        static_assert(has_member_eventLooper<_Class>::value, "Class has not in a event looper!!!");
        m_m = std::make_shared<__Impl<_Class, _Callable>>(object_, fun_);
        m_looper = object_->eventLooper();
    }


    void run(Args&& ... args_)
    {
        m_m->run(std::forward<Args>(args_)...);
    }


    CommonTask* convertTask(Args&& ... args_)
    {
        return m_m->convertTask(std::forward<Args>(args_)...);
    }



    int type() const
    {
        return m_type;
    }

    Looper* eventLooper()
    {
        return m_looper;
    }




private:

    std::shared_ptr<__Impl_base> m_m;
    CONNECT_TYPE m_type;
    Looper* m_looper;


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


    template <typename _Class, typename _Callable>
    bool disconnect(_Class* object_, const _Callable& fun_)
    {
        Slot<Args...>* _t = new Slot<Args...>(object_, fun_);
        m_slots.push_back(_t);
        return true;
    }



    void emit(Args&& ... args_)
    {
        Looper* _cLooper = Looper::currentLooper();
        assert(_cLooper != NULL);
        for(Slot<Args...>* t : m_slots)
        {
            switch (t->type()) {
            case CONNECT_AUTO:
                assert(t->eventLooper() != NULL);
                if (_cLooper == t->eventLooper()) {
                    t->run(std::forward<Args>(args_)...);
                } else {
                    t->eventLooper()->enqueue(t->convertTask(std::forward<Args>(args_)...));
                }
                break;

            case CONNECT_AYNC:
                assert(t->eventLooper() != NULL);
                t->eventLooper()->enqueue(t->convertTask(std::forward<Args>(args_)...));
                break;

            case CONNECT_SYNC:
                t->run(std::forward<Args>(args_)...);
                break;

            default:
                break;
            }
        }
    }

private:
    std::list<Slot<Args...>*> m_slots;


};











DEFINE_NAMESPACE_ZZ_END


#endif // MOOS_SIGNAL_H
