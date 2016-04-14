TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    graphic/ui/moos_graphic_button.cpp \
    graphic/ui/moos_graphic_item.cpp \
    base/core/moos_looper.cpp \
    base/appframework/moos_application.cpp \
    graphic/ui/moos_point.cpp \
    graphic/ui/moos_size.cpp \
    graphic/engine/moos_paintengine.cpp



INCLUDEPATH += base/core/include \
               graphic/ui/include \
               base/appframework/include \
               graphic/engine/include



LIBS += -lpthread -lGL -lglut

DEFINES += _GLIBCXX_USE_NANOSLEEP

HEADERS += \
    graphic/ui/include/moos_graphic_button.h \
    graphic/ui/include/moos_graphic_item.h \
    base/core/include/moos_defines.h \
    base/core/include/moos_looper.h \
    base/core/include/moos_object.h \
    base/core/include/moos_signal.h \
    base/core/include/moos_task.h \
    base/core/include/moos_task_policy.h \
    base/core/include/moos_task_queue.h \
    base/core/include/moos_thread.h \
    base/appframework/include/moos_application.h \
    graphic/ui/include/moos_point.h \
    graphic/ui/include/moos_size.h \
    base/core/include/moos_log.h \
    graphic/engine/include/moos_paintengine.h \
    graphic/engine/include/moos_painter.h
