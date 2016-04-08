TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp

HEADERS += \
    moos_thread.h \
    moos_signal.h \
    moos_task.h \
    moos_task_policy.h \
    moos_task_queue.h \
    moos_looper.h \
    moos_defines.h \
    moos_object.h


LIBS += -pthread

DEFINES += _GLIBCXX_USE_NANOSLEEP
