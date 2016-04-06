#ifndef MOOS_THREAD_H
#define MOOS_THREAD_H

#include <thread>
#include <memory>
#include <atomic>

#include "defines.h"


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
            m_threadHandler->detach();
        }
        return m_running;
    }

    bool stop()
    {
        return m_running = false;
    }

    virtual bool threadRun() = 0;

private:

    void _run()
    {
        while(m_running && threadRun());
    }

    std::shared_ptr<std::thread> m_threadHandler;
    std::atomic_bool m_running;

};

DEFINE_NAMESPACE_ZZ_END

#endif // MOOS_THREAD_H
