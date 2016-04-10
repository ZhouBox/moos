TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    graphic/moos_graphic_button.cpp \
    graphic/moos_graphic_item.cpp \
    base/moos_looper.cpp



INCLUDEPATH += base/include \
               graphic/include



LIBS += -lpthread -lGL -lglut

DEFINES += _GLIBCXX_USE_NANOSLEEP

HEADERS += \
    base/include/moos_defines.h \
    base/include/moos_looper.h \
    base/include/moos_object.h \
    base/include/moos_signal.h \
    base/include/moos_task.h \
    base/include/moos_task_policy.h \
    base/include/moos_task_queue.h \
    base/include/moos_thread.h \
    graphic/include/moos_graphic_item.h \
    graphic/include/moos_graphic_button.h
