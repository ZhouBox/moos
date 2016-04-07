#ifndef MOOS_THREAD_H
#define MOOS_THREAD_H

#include <thread>
#include <memory>
#include <atomic>


#include "defines.h"
#include "looper.h"


DEFINE_NAMESPACE_ZZ_BEGIN


class Thread
{
public:
    Thread()
        : m_running(false)
    {

    }

    bool start()
    {
        if (m_running) {

        }
        else {
            m_running = true;
            m_threadHandler.reset(new std::thread(&Thread::_run, this));
            m_id = m_threadHandler->get_id();
            m_threadHandler->detach();

        }
        return m_running;
    }

    bool stop()
    {
        return m_running = false;
    }


    const std::thread::id& getId() const
    {
        return m_id;
    }

    virtual bool threadRun() = 0;

private:

    void _run()
    {
        (void*)Looper::currentLooper();
        while(m_running && threadRun())
        {
            Looper::currentLooper()->exec_once(zz::THREAD_TICK);
        }
    }

    std::shared_ptr<std::thread> m_threadHandler;
    std::atomic_bool m_running;
    std::thread::id m_id;

};

DEFINE_NAMESPACE_ZZ_END

#endif // MOOS_THREAD_H
