#include <iostream>


#include "moos_task.h"
#include "moos_thread.h"
#include "moos_looper.h"
#include "moos_signal.h"

#include <stdlib.h>


DEFINE_NAMESPACE_ZZ_BEGIN


class InputEventThread : public Thread
{
public:
    InputEventThread(Looper* looper_)
        : m_looper(looper_)
    {

    }

    bool threadRun()
    {
        std::cout << "Input Event Thread id: " << std::this_thread::get_id() << std::endl;

        std::this_thread::sleep_for(std::chrono::milliseconds(rand() % 7 * 1000));
        if (rand() % 2 == 0) {
            m_looper->enqueue(new CommonTask([](){ std::cout << "Common Task: " << std::this_thread::get_id() << "\n"; }));
        }
        else {
            TaskBase* _task = new DelayTask([](){ std::cout << "Delay Task: " << std::this_thread::get_id() << "\n"; });
            _task->setTtl(rand() % 5 * 1000);
            m_looper->enqueue(_task);
        }

        return true;
    }


private:
    Looper* m_looper;

};



auto add(const int t1_, const int t2_) -> decltype(t1_ + t2_)
{
    std::cout << t1_ << "+" << t2_ << "=" << t1_ + t2_ << std::endl;
    return t1_ + t2_;
}


void addfun(const std::thread::id& id_)
{
    Looper* _looper = Looper::getLooper(id_);
    _looper->enqueue(new CommonTask([](){ std::cout << "add fun task id:" << std::this_thread::get_id() << std::endl; }));
}


struct Add
{
    int add(const int t1_, const int t2_)
    {
        std::cout << t1_ << "+" << t2_ << "=" << t1_ + t2_ << std::endl;
        return t1_ + t2_;
    }
};

DEFINE_NAMESPACE_ZZ_END


int main(int argc, char *argv[])
{

    UN_USE(argc);
    UN_USE(argv);









    zz::CommonTask cTask_(zz::add, 1, 2);

    cTask_.run();



    zz::Add _add;

    zz::Signal<int, int> s;
    s.connect(&_add, &zz::Add::add);
    s.emit(1, 2);
    s.emit(1, 2);


    zz::CommonTask c1Task_(std::bind(&zz::Add::add, &_add, 1, 2));

    c1Task_.run();


    zz::Looper* _looper = zz::Looper::currentLooper();

    zz::InputEventThread iTh(_looper);


    iTh.start();

    std::this_thread::sleep_for(std::chrono::seconds(1));



    _looper->enqueue(new zz::CommonTask(zz::addfun, iTh.getId()));


    _looper->exec_once();

    _looper->exec();



    return 0;
}
