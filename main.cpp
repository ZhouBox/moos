#include <iostream>


#include "moos_application.h"

#include "moos_task.h"
#include "moos_thread.h"
#include "moos_looper.h"
#include "moos_signal.h"
#include "moos_object.h"
#include "moos_graphic_button.h"
#include "moos_logger.h"
#include "moos_eventhub.h"

#include <stdlib.h>


DEFINE_NAMESPACE_MOOS_BEGIN



struct Add : public MoosObject
{

    int add(const int t1_, const int t2_)
    {
        std::cout << t1_ << "+" << t2_ << "=" << t1_ + t2_ << std::endl;
        return t1_ + t2_;
    }
};



class InputEventThread : public MoosThread
{
public:
    InputEventThread(MoosLooper* looper_, Add* add_)
        : m_looper(looper_)
    {
        //        s.connect(add_, &Add::add, CONNECT_SYNC);
        MOOS_CONNECT(s, add_, &Add::add);
        MOOS_CONNECT(s, add_, &Add::add);
        _t = add_;

    }

    bool threadRun()
    {
        std::cout << "Input Event Thread id: " << std::this_thread::get_id() << std::endl;

        Add a__;

        std::this_thread::sleep_for(std::chrono::milliseconds(rand() % 7 * 1000));
        if (rand() % 2 == 0) {
            m_looper->enqueue(new MoosCommonTask([](){ std::cout << "Common Task: " << std::this_thread::get_id() << "\n"; }));
        }
        else {
            MoosTaskBase* _task = new MoosDelayTask([](){ std::cout << "Delay Task: " << std::this_thread::get_id() << "\n"; });
            _task->setTtl(rand() % 5 * 1000);
            m_looper->enqueue(_task);
        }

        //        s.emit(rand(), rand());
        MOOS_EMIT(s, rand() % 100, rand() % 100);

        //        s.disconnect(_t, &Add::add);
        //        MOOS_DISCONNECT(s, _t, &Add::add);


        return true;
    }


private:
    MoosLooper* m_looper;
    //    Signal<int, int> s;
    MOOS_SIGNAL(int, int) s;

    Add* _t;

};



auto add(const int t1_, const int t2_) -> decltype(t1_ + t2_)
{
    std::cout << t1_ << "+" << t2_ << "=" << t1_ + t2_ << std::endl;
    return t1_ + t2_;
}


void addfun(const std::thread::id& id_)
{
    auto* _looper = MoosLooper::getLooper(id_);
    _looper->enqueue(new MoosCommonTask([](){ std::cout << "add fun task id:" << std::this_thread::get_id() << std::endl; }));
}




void testBase(int argc, char *argv[])
{

    MOOS_UNUSE(argc);
    MOOS_UNUSE(argv);

    Moos::MoosCommonTask cTask_(Moos::add, 1, 2);

    cTask_.run();


    Moos::Add _add;




    Moos::MoosCommonTask c1Task_(std::bind(&Moos::Add::add, &_add, 1, 2));

    c1Task_.run();


    Moos::MoosLooper* _looper = Moos::MoosLooper::currentLooper();

    Moos::InputEventThread iTh(_looper, &_add);


    iTh.start();

    std::this_thread::sleep_for(std::chrono::seconds(1));



    _looper->enqueue(new Moos::MoosCommonTask(Moos::addfun, iTh.getId()));

   _looper->exec();

}

void testGraphic(int argc, char *argv[])
{
    Moos::MoosApplication app(argc, argv);

    Moos::MoosGraphicButton button;
    button.show();

    app.exec();

}

void testEventHub(int argc, char* argv[])
{
    MoosEventHub hub;
    MoosRawEvent arr[10];
    hub.scanDevices();
//    hub.getEvents(arr, 10);
}


DEFINE_NAMESPACE_MOOS_END


#include <fstream>




int main(int argc, char *argv[])
{

//    Moos::testBase(argc, argv);

//    Moos::testGraphic(argc, argv);

    Moos::testEventHub(argc, argv);

    return 0;
}
