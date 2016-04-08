#ifndef MOOS_SIGNAL_H
#define MOOS_SIGNAL_H

#include <memory>
#include <list>
#include <type_traits>
#include <cassert>
#include <algorithm>

#include "moos_defines.h"
#include "moos_looper.h"

DEFINE_NAMESPACE_ZZ_BEGIN



HAS_MEMBER(eventLooper);

template <typename ... Args>
struct __Impl_Signal_base
{
    virtual ~__Impl_Signal_base() {}
    virtual void run(Args&& ... args_) = 0;
    virtual CommonTask* convertTask(Args&& ... args_) = 0;
};

template <typename _Class, typename _Callable, typename ... Args>
struct __Impl_Signal : public __Impl_Signal_base<Args...>
{
    __Impl_Signal(_Class* object_, const _Callable& fun_)
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

template <typename ... Args>
struct Slot
{


    template <typename _Class, typename _Callable>
    Slot(_Class* object_, const _Callable& fun_, CONNECT_TYPE type_)
        : m_type(type_)
    {
        static_assert(has_member_eventLooper<_Class>::value, "Class has not in a event looper!!!");
        m_m = std::make_shared<__Impl_Signal<_Class, _Callable, Args...>>(object_, fun_);
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




    std::shared_ptr<__Impl_Signal_base<Args...>> m_m;
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
        auto ite = std::find_if(m_slots.begin(), m_slots.end(), FindHelper<_Class, _Callable>(object_, fun_));
        if (ite != m_slots.end()) {
            return false;
        }

        Slot<Args...>* _t = new Slot<Args...>(object_, fun_, type_);
        m_slots.push_back(_t);
        return true;
    }


    template <typename _Class, typename _Callable>
    bool disconnect(_Class* object_, const _Callable& fun_)
    {
        auto ite = std::find_if(m_slots.begin(), m_slots.end(), FindHelper<_Class, _Callable>(object_, fun_));
        if (ite != m_slots.end()) {
            m_slots.erase(ite);
            return true;
        }
        return false;
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


    template <typename _Class, typename _Callable>
    struct FindHelper
    {
        FindHelper(_Class* object_, const _Callable& fun_)
            : m_object(object_)
            , m_Mfun(fun_)
        {

        }

        bool operator()(Slot<Args...>* val_) const
        {
            __Impl_Signal<_Class, _Callable, Args...>* _tImpl = static_cast<__Impl_Signal<_Class, _Callable, Args...>*>(val_->m_m.get());
            return (m_object == _tImpl->m_object) && (m_Mfun == _tImpl->m_Mfun);
        }

        bool operator()(Slot<Args...>* val_)
        {
            __Impl_Signal<_Class, _Callable, Args...>* _tImpl = static_cast<__Impl_Signal<_Class, _Callable, Args...>*>(val_->m_m.get());
            return (m_object == _tImpl->m_object) && (m_Mfun == _tImpl->m_Mfun);
        }


        _Class* m_object;
        _Callable m_Mfun;

    };


};











DEFINE_NAMESPACE_ZZ_END


#define MOOS_SIGNAL(...) zz::Signal<__VA_ARGS__>

#define MOOS_CONNECT(Signal, ...) Signal.connect(__VA_ARGS__)

#define MOOS_DISCONNECT(Signal, ...) Signal.disconnect(__VA_ARGS__)


#define MOOS_EMIT(Signal, ...) Signal.emit(__VA_ARGS__)




#endif // MOOS_SIGNAL_H
