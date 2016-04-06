#ifndef LOOPER_H
#define LOOPER_H

#include "defines.h"

#include "task_queue.h"


DEFINE_NAMESPACE_ZZ_BEGIN


class Looper
{
public:
    void enqueue(TaskBase* t_)
    {
        return m_queue.enqueue(t_);
    }

    bool dequeue(TaskBase*& t_, int ms_ = -1)
    {
        return m_queue.dequeue(t_, ms_);
    }

    void exec_once()
    {
        TaskBase* _task = NULL;
        if (m_queue.dequeue(_task)) {
            if (_task->type() == TASK_COMMON) {
                _task->run();
                delete _task;
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
    }


    void exec()
    {
        while(true) {
            TaskBase* _task = NULL;
            if (m_queue.dequeue(_task)) {
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
    TaskQueue m_queue;
};


DEFINE_NAMESPACE_ZZ_END

#endif // LOOPER_H
