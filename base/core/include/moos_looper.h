#ifndef MOOS_LOOPER_H
#define MOOS_LOOPER_H


#include <map>
#include <thread>

#include "moos_defines.h"

#include "moos_task_queue.h"


DEFINE_NAMESPACE_MOOS_BEGIN


class MoosLooper
{
public:


    static MoosLooper* currentLooper()
    {
        return getLooper(std::this_thread::get_id());
    }

    static MoosLooper* getLooper(const std::thread::id& id_)
    {
        auto _looper = m_loopers.find(id_);
        MoosLooper *_re = NULL;
        if (_looper != m_loopers.end()) {
            _re = (*_looper).second;
        }
        else {
            std::mutex _mutex;
            std::unique_lock<std::mutex> _lock(_mutex);
            _re = new MoosLooper();
            m_loopers[id_] = _re;
        }
        return _re;
    }






    void enqueue(MoosTaskBase* t_)
    {
        return m_queue.enqueue(t_);
    }

    bool dequeue(MoosTaskBase*& t_, int ms_ = -1)
    {
        return m_queue.dequeue(t_, ms_);
    }

    void exec_once(int ms_ = -1)
    {
        MoosTaskBase* _task = NULL;
        do {
            if (m_queue.dequeue(_task, ms_)) {
                if (_task->type() == TASK_COMMON) {
                    _task->run();
                    delete _task;
                    break;
                }

                if (_task->type() == TASK_DELAY) {
                    if (_task->ttl() == 0) {
                        _task->run();
                        delete _task;
                    }
                    else {
                        m_queue.enqueue(_task);
                    }
                }
            }
        } while(0);
    }


    int exec(int ms_ = -1)
    {
        int _re = 1;
        while(m_isRunning) {
            MoosTaskBase* _task = NULL;
            if (m_queue.dequeue(_task, ms_)) {
                if (_task->type() == TASK_COMMON) {
                    _task->run();
                    delete _task;
                    continue;
                }

                if (_task->type() == TASK_DELAY) {
                    if (_task->ttl() == 0) {
                        _task->run();
                        delete _task;
                        continue;
                    }
                    else {
                        m_queue.enqueue(_task);
                    }
                }
            }
        }

        return _re;

    }

    void exit()
    {
        m_isRunning = false;
    }

private:
    MoosLooper()
        : m_isRunning(true)
    {

    }



    MoosTaskQueue m_queue;
    static std::map<std::thread::id, MoosLooper*> m_loopers;
    bool m_isRunning;

};




DEFINE_NAMESPACE_MOOS_END

#endif // MOOS_LOOPER_H
