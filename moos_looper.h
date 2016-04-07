#ifndef LOOPER_H
#define LOOPER_H


#include <map>
#include <thread>

#include "moos_defines.h"

#include "moos_task_queue.h"


DEFINE_NAMESPACE_ZZ_BEGIN


class Looper
{
public:


    static Looper* currentLooper()
    {
        auto _looper = m_loopers.find(std::this_thread::get_id());
        Looper *_re = NULL;
        if (_looper != m_loopers.end()) {
            _re = (*_looper).second;
        }
        else {
            _re = new Looper();
            m_loopers[std::this_thread::get_id()] = _re;
        }
        return _re;
    }

    static Looper* getLooper(const std::thread::id& id_)
    {
        auto _looper = m_loopers.find(id_);
        Looper *_re = NULL;
        if (_looper != m_loopers.end()) {
            _re = (*_looper).second;
        }
        else {
            _re = new Looper();
            m_loopers[id_] = _re;
        }
        return _re;
    }






    void enqueue(TaskBase* t_)
    {
        return m_queue.enqueue(t_);
    }

    bool dequeue(TaskBase*& t_, int ms_ = -1)
    {
        return m_queue.dequeue(t_, ms_);
    }

    void exec_once(int ms_ = -1)
    {
        TaskBase* _task = NULL;
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


    void exec(int ms_ = -1)
    {
        while(true) {
            TaskBase* _task = NULL;
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
    }

private:
    Looper() {}



    TaskQueue m_queue;
    static std::map<std::thread::id, Looper*> m_loopers;

};

std::map<std::thread::id, Looper*> Looper::m_loopers = std::map<std::thread::id, Looper*>();


DEFINE_NAMESPACE_ZZ_END

#endif // LOOPER_H
