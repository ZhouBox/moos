#ifndef MOOS_THREAD_H
#define MOOS_THREAD_H

#include <thread>
#include <memory>
#include <atomic>


#include "moos_defines.h"
#include "moos_looper.h"


DEFINE_NAMESPACE_MOOS_BEGIN


class MoosThread
{
public:
    MoosThread()
        : m_running(false)
    {

    }


    virtual ~MoosThread()
    {
        if (m_running) {
            m_running = false;
        }
    }

    bool start()
    {
        if (m_running) {

        }
        else {
            m_running = true;
            m_threadHandler.reset(new std::thread(&MoosThread::_run, this));
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
        (void*)MoosLooper::currentLooper();
        while(m_running && threadRun())
        {
            MoosLooper::currentLooper()->exec_once(THREAD_TICK);
        }
    }

    std::shared_ptr<std::thread> m_threadHandler;
    std::atomic_bool m_running;
    std::thread::id m_id;

};

DEFINE_NAMESPACE_MOOS_END

#endif // MOOS_THREAD_H
