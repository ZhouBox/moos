#ifndef MOOS_TASK_QUEUE_H
#define MOOS_TASK_QUEUE_H

#include <list>
#include <condition_variable>
#include <mutex>


#include "moos_defines.h"


DEFINE_NAMESPACE_MOOS_BEGIN


template <typename T>
class MoosQueue
{
public:

    void enqueue(const T& t_)
    {
        std::unique_lock<std::mutex> _lock(m_mutex);
        m_queue.push_back(t_);
        m_cv.notify_all();

    }

    bool dequeue(T& t_, int ms_ = -1)
    {
        bool _re = false;
        std::unique_lock<std::mutex> _lock(m_mutex);
        if (m_queue.empty()) {
            if (ms_ < 0) {
                m_cv.wait(_lock);
            }
            else {
                m_cv.wait_until(_lock, std::chrono::system_clock::now() + std::chrono::milliseconds(ms_));
            }
        }
        else {
            t_ = m_queue.front();
            m_queue.pop_front();
            _re = true;

        }

        return _re;
    }

private:
    std::condition_variable m_cv;
    std::mutex m_mutex;
    std::list<T> m_queue;

};



#include "moos_task.h"

typedef MoosQueue<MoosTaskBase*> MoosTaskQueue;







DEFINE_NAMESPACE_MOOS_END

#endif // MOOS_TASK_QUEUE_H
