#ifndef TASK_H
#define TASK_H

#include <memory>

#include "defines.h"
#include "task_policy.h"


DEFINE_NAMESPACE_ZZ_BEGIN





struct TaskBase
{
    virtual ~TaskBase() {}
    virtual void run() = 0;
    virtual int ttl() const = 0;
    virtual int type() const = 0;
    virtual void setTtl(int ms_) = 0;

};


template <typename _Policy>
class Task : public TaskBase
{
    struct __Impl_base;
    typedef std::shared_ptr<__Impl_base> __share_base_type;



    struct __Impl_base
    {
        virtual ~__Impl_base() {}
        virtual void _run() = 0;
    };

    template <typename _Callable>
    struct __Impl : public __Impl_base
    {
        _Callable m_fun;
        __Impl(_Callable&& f_)
            : m_fun(std::forward<_Callable>(f_))
        {

        }

        void _run()
        {
            m_fun();
        }
    };

public:
    template <typename _Callable, typename ... Args>
    Task(_Callable&& f_, Args&& ... args)
    {
        m_Mfun = _make_routine(std::bind(std::forward<_Callable>(f_), std::forward<Args>(args)...));
    }


    void run()
    {
        m_Mfun->_run();
    }


    int ttl() const
    {
        return m_taskPolicy.ttl().count();
    }

    int type() const
    {
        return m_taskPolicy.type();

    }

    void setTtl(int ms_)
    {
        m_taskPolicy.setTtl(ms(ms_));
    }


private:

    template <typename _Callable>
    std::shared_ptr<__Impl<_Callable>>
    _make_routine(_Callable&& f_)
    {
        return std::make_shared<__Impl<_Callable>>(std::forward<_Callable>(f_));
    }

    __share_base_type m_Mfun;

    _Policy m_taskPolicy;

};


typedef Task<CommonTaskPolicy> CommonTask;
typedef Task<DelayTaskPolicy> DelayTask;


DEFINE_NAMESPACE_ZZ_END



#endif // TASK_H
