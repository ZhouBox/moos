TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp

HEADERS += \
    defines.h \
    task.h \
    task_policy.h \
    moos_thread.h \
    task_queue.h \
    looper.h


LIBS += -pthread

DEFINES +=
