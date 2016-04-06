#ifndef TASK_POLICY_H
#define TASK_POLICY_H

#include "defines.h"




DEFINE_NAMESPACE_ZZ_BEGIN

struct TaskPolicy
{
    virtual ms ttl() const
    {
        return ms(0);
    }

    virtual int type() const  = 0;

    virtual void setTtl(ms ms_)
    {
        UN_USE(ms_);
    }



};



class CommonTaskPolicy : public TaskPolicy
{
public:
    int type() const
    {
        return TASK_COMMON;
    }



};




class DelayTaskPolicy : public TaskPolicy
{
public:

    DelayTaskPolicy()
        : m_delay(std::chrono::system_clock::now())
    {

    }

    int type() const
    {
        return TASK_DELAY;
    }


    ms ttl() const
    {
        std::chrono::system_clock::duration _tt = m_delay - std::chrono::system_clock::now();
        ms _t = std::chrono::duration_cast<std::chrono::milliseconds>(_tt);
        if (_t.count() > 0) {
            return _t;
        }
        return ms(0);

    }

    void setTtl(ms ms_)
    {
        m_delay = std::chrono::system_clock::now() + ms_;

    }


private:

    std::chrono::system_clock::time_point m_delay;


};


DEFINE_NAMESPACE_ZZ_END

#endif // TASK_POLICY_H
